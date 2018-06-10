#include "Qt_Code_Reader.h"
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
        if (line.startsWith(Qt_Code_Strings::STRING_RETURN)) atEnd = true;
        if (!this->Get_Offset_From_Line(line, offset)) return false;


        //TODO: Write this... (get the value!)
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
