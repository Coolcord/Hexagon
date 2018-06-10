#include "Patch_Writer.h"
#include "Patch_Strings.h"
#include <assert.h>

const static int PATCH_TEXT_WIDTH = 50;

Patch_Writer::Patch_Writer(QFile *file) {
    assert(file);
    assert(file->isOpen() && file->isWritable());
    this->stream = new QTextStream(file);
}

Patch_Writer::~Patch_Writer() {
    delete this->stream;
}

bool Patch_Writer::Write_Break_Line() {
    *this->stream << Patch_Strings::STRING_COMMENT << QByteArray('=', PATCH_TEXT_WIDTH-1) << Patch_Strings::STRING_NEW_LINE;
    //TODO: Write this...
}

bool Patch_Writer::Write_Checksum(const QString &checksum) {
    *this->stream << Patch_Strings::STRING_CHECKSUM << " " << checksum << Patch_Strings::STRING_NEW_LINE;
    //TODO: Write this...
}

bool Patch_Writer::Write_Comment(const QString &comment) {
    *this->stream << Patch_Strings::STRING_COMMENT << " " << comment << Patch_Strings::STRING_NEW_LINE;
    //TODO: Write this...
}

bool Patch_Writer::Write_Header() {
    //TODO: Write this...
}

bool Patch_Writer::Write_Next_Patch(const qint64 offset, const QString &value) {
    //TODO: Write this...
}

bool Patch_Writer::Write_Next_Patch(const qint64 offset, const QByteArray &value) {
    //TODO: Write this...
}
