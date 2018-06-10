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
    this->stream = new QTextStream(file);
}

Qt_Code_Reader::~Qt_Code_Reader() {
    delete this->stream;
}

bool Qt_Code_Reader::Read_Next_Patch(qint64 &offset, QString &value) {
    bool atEnd = false;
    while (!this->stream->atEnd() && !atEnd) {
        ++this->currentLineNum;
        QString line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.startsWith(Qt_Code_Strings::STRING_RETURN)) atEnd = true;
        bool singleValue = line.contains(Qt_Code_Strings::STRING_BYTE_ARRAY);
        bool multiValue = line.contains(Qt_Code_Strings::STRING_FROM_HEX);
        if (!singleValue && !multiValue) continue; //just ignore this line
        assert(singleValue != multiValue);
        if (!this->Get_Offset_From_Line(line, offset)) return false;
        if (singleValue && !this->Get_Single_Value_From_Line(line, value)) return false;
        if (multiValue && !this->Get_Multi_Value_From_Lines(line, value)) return false;
    }
    return true;
}

bool Qt_Code_Reader::Read_Next_Patch(qint64 &offset, QByteArray &value) {
    QString valueString = this->valueManipulator->Convert_QByteArray_To_QString(value);
    if (!this->Read_Next_Patch(offset, valueString)) return false;
    return this->valueManipulator->Convert_QString_To_QByteArray(valueString, value);
}

bool Qt_Code_Reader::Get_Offset_From_Line(const QString &line, qint64 &offset) {
    QStringList strings = line.split(',');
    if (strings.size() < 2) return false;
    strings = strings.at(0).split('(');
    if (strings.size() < 2) return false;
    QString numString = strings.last();
    bool valid = false;
    bool hex = false;
    if (numString.startsWith("0x")) {
        hex = true;
        numString = numString.split('x').last();
    }
    qint64 tmpOffset = 0;
    if (hex) numString.toULongLong(&valid, 0x10);
    else numString.toULongLong(&valid, 10);
    if (!valid) return false;
    offset = tmpOffset;
    return true;
}

bool Qt_Code_Reader::Get_Multi_Value_From_Lines(const QString &line, QString &value) {
    //TODO: Write this...
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
        valueNumber = valueNumber.remove(Patch_Strings::STRING_HEX_IDENTIFIER.size(), valueNumber.size()-Patch_Strings::STRING_HEADER.size());
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
