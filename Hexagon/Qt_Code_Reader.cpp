#include "Qt_Code_Reader.h"
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
    while (!this->stream->atEnd()) {
        ++this->currentLineNum;
        QString line = this->stream->readLine().trimmed();
        //TODO: Write this...
    }
}

bool Qt_Code_Reader::Read_Next_Patch(qint64 &offset, QByteArray &value) {
    QString valueString = this->valueManipulator->Convert_QByteArray_To_QString(value);
    if (!this->Read_Next_Patch(offset, valueString)) return false;
    return this->valueManipulator->Convert_QString_To_QByteArray(valueString, value);
}
