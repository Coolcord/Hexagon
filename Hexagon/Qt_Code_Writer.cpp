#include "Qt_Code_Writer.h"
#include <assert.h>

Qt_Code_Writer::Qt_Code_Writer(QFile *file) {
    assert(file);
    assert(file->isOpen() && file->isWritable());
    this->stream = new QTextStream(file);
}

Qt_Code_Writer::~Qt_Code_Writer() {
    delete this->stream;
}

bool Qt_Code_Writer::Write_Next_Patch(const qint64 offset, const QString &value) {

}

bool Qt_Code_Writer::Write_Next_Patch(const qint64 offset, const QByteArray &value) {

}
