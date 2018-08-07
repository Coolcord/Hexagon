#include "Qt_Code_Reader.h"
#include "Patch_Strings.h"
#include "Qt_Code_Strings.h"
#include "Value_Manipulator.h"
#include <assert.h>

Qt_Code_Reader::Qt_Code_Reader(QFile *file, Value_Manipulator *valueManipulator) {
    assert(file);
    assert(file->isOpen() && file->isReadable());
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->currentLineNum = 0;
    this->currentLine = QString();
    this->stream = new QTextStream(file);
}

Qt_Code_Reader::~Qt_Code_Reader() {
    delete this->stream;
}

int Qt_Code_Reader::Get_Current_Line_Num() {
    return this->currentLineNum;
}

bool Qt_Code_Reader::Read_Next_Patch(qint64 &offset, QString &value, bool &parseError) {
    parseError = true;
    bool singleValue = false, multiValue = false;
    QString line = QString();
    do {
        line = this->Get_Next_Line_After_Comments();
        if (line.isEmpty()) {
            parseError = false;
            return false;
        }
        singleValue = line.contains(Qt_Code_Strings::STRING_BYTE_ARRAY);
        multiValue = line.contains(Qt_Code_Strings::STRING_FROM_HEX);
    } while (!singleValue && !multiValue);
    if (singleValue || multiValue) {
        if (!this->Get_Offset_From_Line(line, offset)) return false;
        if (singleValue && !this->Get_Single_Value_From_Line(line, value)) return false;
        if (multiValue && !this->Get_Multi_Value_From_Lines(line, value)) return false;
        parseError = false;
        return true;
    } else {
        parseError = true;
        return false;
    }
}

bool Qt_Code_Reader::Read_Next_Patch(qint64 &offset, QByteArray &value, bool &parseError) {
    parseError = true;
    QString valueString = this->valueManipulator->Convert_QByteArray_To_QString(value);
    if (!this->Read_Next_Patch(offset, valueString, parseError)) return false;
    return this->valueManipulator->Convert_QString_To_QByteArray(valueString, value);
}

QString Qt_Code_Reader::Get_Next_Line_After_Comments() {
    //Return what's in the buffer if it is in use
    if (!this->currentLine.isEmpty()) {
        QString line = this->currentLine;
        this->currentLine.clear();
        return line;
    }

    //Start searching for the next line
    QString line = QString();
    while (line.isEmpty() && !line.startsWith(Qt_Code_Strings::STRING_COMMENT)) {
        if (this->stream->atEnd()) return QString();
        ++this->currentLineNum;
        line = this->stream->readLine().trimmed();
    }
    return line;
}

bool Qt_Code_Reader::Get_Offset_From_Line(const QString &line, qint64 &offset) {
    QStringList strings = line.split(',');
    if (strings.size() < 2) return false;
    strings = strings.at(0).split(Qt_Code_Strings::STRING_WRITE_BYTES_TO_OFFSET);
    QString numString = strings.last();
    bool valid = false;
    bool hex = false;
    if (numString.startsWith("0x")) {
        hex = true;
        numString = numString.split('x').last();
    }
    qint64 tmpOffset = 0;
    if (hex) tmpOffset = numString.toLongLong(&valid, 0x10);
    else tmpOffset = numString.toLongLong(&valid, 10);
    if (!valid) return false;
    offset = tmpOffset;
    return true;
}

bool Qt_Code_Reader::Get_Multi_Value_From_Lines(const QString &firstLine, QString &value) {
    QStringList strings = firstLine.split("\"");
    if (strings.size() != 3) return false;
    if (!this->valueManipulator->Is_Line_Hex_String(strings.at(1))) return false;
    value = strings.at(1);

    //Pull any additional lines
    while (!this->stream->atEnd()) {
        ++this->currentLineNum;
        QString line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.contains(Qt_Code_Strings::STRING_WRITE_BYTES_TO_OFFSET)) { //this line is not part of the value
            this->currentLine = line;
            return true;
        } else {
            strings = line.split("\"");
            if (strings.size() != 3) return false;
            if (!this->valueManipulator->Is_Line_Hex_String(strings.at(1))) return false;
            value += strings.at(1);
        }
    }
    return true;
}

bool Qt_Code_Reader::Get_Single_Value_From_Line(const QString &line, QString &value) {
    QStringList strings = line.split(Qt_Code_Strings::STRING_BYTE_ARRAY);
    if (strings.size() < 2) return false;
    strings = strings.at(1).split(')');
    if (strings.size() < 2) return false;
    strings = strings.at(0).split(Qt_Code_Strings::STRING_COMMA);
    if (strings.size() < 2) return false;
    QString numberOfValues = strings.at(0);
    QString valueNumber = strings.at(1);
    bool isHex = false;
    if (valueNumber.startsWith(Patch_Strings::STRING_HEX_IDENTIFIER)) {
        isHex = true;
        valueNumber = valueNumber.remove(0, Patch_Strings::STRING_HEX_IDENTIFIER.size());
    }
    bool valid = false;
    int num = 0;
    if (isHex) num = numberOfValues.toInt(&valid, 0x10);
    else num = numberOfValues.toInt(&valid, 10);
    if (!valid) return false;

    //Combine the Number of Values Specified
    value = QString();
    for (int i = 0; i < num; ++i) {
        value += valueNumber;
    }
    return true;
}
