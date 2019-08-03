#include "Patch_Reader.h"
#include "Patch_Strings.h"
#include "Value_Manipulator.h"
#include <assert.h>

Patch_Reader::Patch_Reader(QFile *file, Value_Manipulator *valueManipulator) {
    assert(file);
    assert(file->isOpen() && file->isReadable());
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->currentLineNum = 0;
    this->currentLine = QString();
    this->stream = new QTextStream(file);
}

Patch_Reader::Patch_Reader(const QByteArray &patchBytes, Value_Manipulator *valueManipulator) {
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->currentLineNum = 0;
    this->currentLine = QString();
    this->stream = new QTextStream(patchBytes, QIODevice::ReadOnly);
}

Patch_Reader::~Patch_Reader() {
    delete this->stream;
}

int Patch_Reader::Get_Current_Line_Num() {
    return this->currentLineNum;
}

bool Patch_Reader::Get_Checksum(QString &checksum) {
    QString line = this->Get_Next_Line_After_Comments();
    if (line.startsWith(Patch_Strings::STRING_OFFSET) || line.startsWith(Patch_Strings::STRING_SIZE)) {
        this->stream->seek(0); //go back to the beginning... we read too far ahead
        this->currentLineNum = 0;
        checksum = Patch_Strings::STRING_SKIP_CHECKSUM;
        return true; //no checksum
    }
    if (!line.startsWith(Patch_Strings::STRING_CHECKSUM)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    checksum = values.at(1).trimmed();
    return true;
}

bool Patch_Reader::Get_Size(qint64 &size) {
    size = 0;
    QString line = this->Get_Next_Line_After_Comments();
    if (line.startsWith(Patch_Strings::STRING_OFFSET)) {
        this->stream->seek(0); //go back to the beginning... we read too far ahead
        this->currentLineNum = 0;
        return true; //no size specified
    }
    if (!line.startsWith(Patch_Strings::STRING_SIZE)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    QString sizeLine = values.at(1).trimmed();
    bool isNegative = sizeLine.startsWith("-");
    if (isNegative || sizeLine.startsWith("+")) sizeLine.remove(0, 1);
    bool isHex = sizeLine.startsWith("0x");
    bool valid = false;
    if (isHex) size = sizeLine.toLongLong(&valid, 0x10);
    else size = sizeLine.toLongLong(&valid, 10);
    if (isNegative) size = 0-size;
    return valid;
}

bool Patch_Reader::Get_Next_Offset_And_Value(qint64 &offset, QByteArray &value, bool &parseError) {
    //Get the Offset
    parseError = true;
    QString line = QString();
    if (this->currentLineNum > 0) {
        line = this->Get_Next_Line_After_Comments();
    } else { //we might have to read the header again in some circumstances
        bool headerParseError = false;
        line = this->Get_Next_Line_After_Header(headerParseError);
        if (headerParseError) {
            parseError = true;
            return false;
        }
    }
    if (line.isEmpty()) { //nothing left to read
        parseError = false;
        return false;
    }
    if (!line.startsWith(Patch_Strings::STRING_OFFSET)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    QString offsetString = values.at(1);
    offsetString = this->valueManipulator->Trim_Hex_Identifier(offsetString);
    bool valid = false;
    offset = offsetString.toLongLong(&valid, 0x10);
    if (!valid) return false;

    //Get the Value
    if (!this->Parse_Value(value)) return false;
    parseError = false;
    return true;
}

QString Patch_Reader::Get_Next_Line_After_Comments() {
    //Check the buffer first
    if (!this->currentLine.isEmpty()) {
        QString line = this->currentLine;
        this->currentLine.clear();
        return line;
    }

    //Start searching
    QString line = QString();
    while (!this->stream->atEnd()) {
        ++this->currentLineNum;
        line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.startsWith(Patch_Strings::STRING_COMMENT)) continue;
        else return line;
    }
    return QString(); //nothing left to read
}

QString Patch_Reader::Get_Next_Line_After_Header(bool &parseError) {
    parseError = false;
    bool foundChecksum = false, foundSize = false;
    QString line = this->Get_Next_Line_After_Comments();
    while (!line.isEmpty()) {
        if (line.startsWith(Patch_Strings::STRING_OFFSET)) {
            parseError = false;
            return line;
        }
        if (line.startsWith(Patch_Strings::STRING_CHECKSUM)) {
            if (foundChecksum) parseError = true;
            foundChecksum = true;
        }
        if (line.startsWith(Patch_Strings::STRING_SIZE)) {
            if (foundSize) parseError = true;
            foundSize = true;
        }
        if (parseError) return line;
        line = this->Get_Next_Line_After_Comments();
    }
    parseError = false;
    return line;
}

bool Patch_Reader::Parse_Value(QByteArray &value) {
    //Parse the First Line
    QString line = this->Get_Next_Line_After_Comments();
    if (!line.startsWith(Patch_Strings::STRING_VALUE)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    QString valueString = values.at(1);
    valueString = this->valueManipulator->Trim_Hex_Identifier(valueString);
    if (!this->valueManipulator->Is_Line_Hex_String(valueString)) return false;
    QTextStream valueStream(&valueString);

    //Parse Any Additional Lines
    while (!this->stream->atEnd()) {
        ++this->currentLineNum;
        line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.startsWith(Patch_Strings::STRING_COMMENT)) continue;
        if (line.startsWith(Patch_Strings::STRING_OFFSET)) {
            this->currentLine = line; //throw the line in a buffer and access it later
            break;
        } else {
            line = this->valueManipulator->Trim_Hex_Identifier(line);
            if (!this->valueManipulator->Is_Line_Hex_String(line)) return false;
            valueStream << line;
        }
    }
    valueStream.flush();
    return this->valueManipulator->Convert_QString_To_QByteArray(valueString, value);
}
