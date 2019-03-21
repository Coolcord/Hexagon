#include "File_Writer.h"
#include "Value_Manipulator.h"
#include <assert.h>

File_Writer::File_Writer(QFile *file, Value_Manipulator *valueManipulator) {
    assert(file);
    assert(valueManipulator);
    this->file = file;
    this->valueManipulator = valueManipulator;
}

bool File_Writer::Write_Bytes_To_Offset(qint64 offset, const QByteArray &bytes, bool &seekValid) {
    if (!this->file->seek(offset)) {
        seekValid = false;
        return false;
    }
    seekValid = true;
    return this->file->write(bytes) == bytes.size();
}

bool File_Writer::Write_Bytes_To_Offset(qint64 offset, const QString &value, bool &seekValid) {
    QByteArray bytes;
    if (!this->valueManipulator->Convert_QString_To_QByteArray(value, bytes)) return false;
    return this->Write_Bytes_To_Offset(offset, bytes, seekValid);
}

bool File_Writer::Pad_Or_Trim_File(qint64 amount) {
    if (amount == 0) return true; //nothing to do
    if (amount < 0) { //trim
        return this->file->resize(this->file->size()-amount);
    } else { //pad
        return this->file->resize(this->file->size()+amount);
    }
}
