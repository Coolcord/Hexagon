#include "Qt_Code_Writer.h"
#include "Patch_Strings.h"
#include "Qt_Code_Strings.h"
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

bool Qt_Code_Writer::Write_Last_Patch(const qint64 offset, const QString &value) {
    return this->Write_Patch(offset, value, true);
}

bool Qt_Code_Writer::Write_Last_Patch(const qint64 offset, const QByteArray &value) {
    return this->Write_Last_Patch(offset, this->valueManipulator->Convert_QByteArray_To_QString(value));
}

bool Qt_Code_Writer::Write_Next_Patch(const qint64 offset, const QString &value) {
    return this->Write_Patch(offset, value, false);
}

bool Qt_Code_Writer::Write_Next_Patch(const qint64 offset, const QByteArray &value) {
    return this->Write_Next_Patch(offset, this->valueManipulator->Convert_QByteArray_To_QString(value));
}

bool Qt_Code_Writer::Write_Patch(const qint64 offset, const QString &value, bool lastPatch) {
    if (value.size()%2 == 1) return false;
    bool oneByte = value.size()==2;
    if (lastPatch) *this->stream << Qt_Code_Strings::STRING_RETURN;
    else *this->stream << Qt_Code_Strings::STRING_IF_NOT;
    *this->stream << Qt_Code_Strings::STRING_WRITE_BYTES_TO_OFFSET << Patch_Strings::STRING_HEX_IDENTIFIER << QString::number(offset, 0x10);
    if (oneByte) {
        *this->stream << Qt_Code_Strings::STRING_BYTE_ARRAY << "1" << Qt_Code_Strings::STRING_COMMA << Patch_Strings::STRING_HEX_IDENTIFIER
                      << value << Qt_Code_Strings::STRING_ENDING_PARENTHESES;
    } else {
        *this->stream << Qt_Code_Strings::STRING_FROM_HEX << "\"";
        if (!this->Write_Value_Lines(value)) return false;
        *this->stream << "\"" << Qt_Code_Strings::STRING_TO_LATIN;
    }
    if (lastPatch) *this->stream << Qt_Code_Strings::STRING_RETURN << Patch_Strings::STRING_NEW_LINE;
    else *this->stream << Qt_Code_Strings::STRING_RETURN_FALSE << Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Qt_Code_Writer::Write_Value_Lines(const QString &value) {
    //TODO: Write this...
}
