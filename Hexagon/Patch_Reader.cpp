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
    this->stream = new QTextStream(file);
}

Patch_Reader::Patch_Reader(const QByteArray &patchBytes, Value_Manipulator *valueManipulator) {
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->currentLineNum = 0;
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
    if (!line.startsWith(Patch_Strings::STRING_CHECKSUM)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    checksum = values.at(1);
    return true;
}

bool Patch_Reader::Get_Next_Offset_And_Value(qint64 &offset, QByteArray &value, bool &parseError) {
    //Get the Offset
    parseError = true;
    QString line = this->Get_Next_Line_After_Comments();
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
    offset = offsetString.toULongLong(&valid, 0x10);
    if (!valid) return false;

    //Get the Value
    if (!this->Parse_Value(value)) return false;
    parseError = false;
    return true;
}

QString Patch_Reader::Get_Next_Line_After_Comments() {
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

bool Patch_Reader::Parse_Value(QByteArray &value) {
    //Parse the First Line
    QString line = this->Get_Next_Line_After_Comments();
    if (!line.startsWith(Patch_Strings::STRING_VALUE)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    QString valueString = values.at(1);
    valueString = this->valueManipulator->Trim_Hex_Identifier(valueString);
    if (!this->valueManipulator->Is_Line_Hex_String(valueString)) return false;

    //Parse Any Additional Lines
    while (!this->stream->atEnd()) {
        qint64 posBefore = this->stream->pos();
        ++this->currentLineNum;
        line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.startsWith(Patch_Strings::STRING_COMMENT)) continue;
        if (line.startsWith(Patch_Strings::STRING_OFFSET)) {
            assert(this->stream->seek(posBefore));
            --this->currentLineNum;
            break;
        } else {
            line = this->valueManipulator->Trim_Hex_Identifier(line);
            if (!this->valueManipulator->Is_Line_Hex_String(line)) return false;
            valueString += line;
        }
    }
    return this->valueManipulator->Convert_QString_To_QByteArray(valueString, value);
}
