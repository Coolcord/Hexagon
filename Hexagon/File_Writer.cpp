#include "File_Writer.h"
#include "Value_Manipulator.h"
#include <assert.h>

File_Writer::File_Writer(QFile *file, Value_Manipulator *valueManipulator) {
    assert(file);
    assert(valueManipulator);
    this->file = file;
    this->valueManipulator = valueManipulator;
}

bool File_Writer::Write_Bytes_To_Offset(qint64 offset, const QByteArray &bytes) {
    if (!this->file->seek(offset)) return false;
    return this->file->write(bytes) == bytes.size();
}

bool File_Writer::Write_Bytes_To_Offset(qint64 offset, const QString &value) {
    QByteArray bytes;
    if (!this->valueManipulator->Convert_QString_To_QByteArray(value, bytes)) return false;
    return this->Write_Bytes_To_Offset(offset, bytes);
}
