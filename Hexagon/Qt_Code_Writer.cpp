#include "Qt_Code_Writer.h"
#include "Value_Manipulator.h"
#include <assert.h>

Qt_Code_Writer::Qt_Code_Writer(QFile *file, Value_Manipulator *valueManipulator) {
    assert(file);
    assert(file->isOpen() && file->isWritable());
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->stream = new QTextStream(file);
}

Qt_Code_Writer::~Qt_Code_Writer() {
    this->stream->flush();
    delete this->stream;
}

bool Qt_Code_Writer::Write_Next_Patch(const qint64 offset, const QString &value) {

}

bool Qt_Code_Writer::Write_Next_Patch(const qint64 offset, const QByteArray &value) {
    return this->Write_Next_Patch(offset, this->valueManipulator->Convert_QByteArray_To_QString(value));
}
