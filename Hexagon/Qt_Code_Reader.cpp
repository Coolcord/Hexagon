#include "Qt_Code_Reader.h"
#include <assert.h>

Qt_Code_Reader::Qt_Code_Reader(QFile *file) {
    assert(file);
    assert(file->isOpen() && file->isReadable());
    this->currentLineNum = 0;
    this->stream = new QTextStream(file);
}

Qt_Code_Reader::~Qt_Code_Reader() {
    delete this->stream;
}

bool Qt_Code_Reader::Read_Next_Patch(qint64 &offset, QString &value) {

}

bool Qt_Code_Reader::Read_Next_Patch(qint64 &offset, QByteArray &value) {

}
