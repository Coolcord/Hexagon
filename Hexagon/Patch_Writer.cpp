#include "Patch_Writer.h"
#include "Patch_Strings.h"
#include "Value_Manipulator.h"
#include <assert.h>
#include <QDate>

const static int PATCH_TEXT_WIDTH = 50;

Patch_Writer::Patch_Writer(QFile *file, Value_Manipulator *valueManipulator) {
    assert(file);
    assert(file->isOpen() && file->isWritable());
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->numDigitsInOffset = QString::number(file->size(), 0x10).size();
    this->stream = new QTextStream(file);
}

Patch_Writer::~Patch_Writer() {
    this->stream->flush();
    delete this->stream;
}

bool Patch_Writer::Write_Break_Line() {
    *this->stream << Patch_Strings::STRING_COMMENT << QByteArray('=', PATCH_TEXT_WIDTH-1) << Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Patch_Writer::Write_Checksum(const QString &checksum) {
    *this->stream << Patch_Strings::STRING_CHECKSUM << " " << checksum << Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Patch_Writer::Write_Comment(const QString &comment) {
    *this->stream << Patch_Strings::STRING_COMMENT << " " << comment << Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Patch_Writer::Write_Header() {
    *this->stream << Patch_Strings::STRING_COMMENT << " " << Patch_Strings::STRING_HEADER << Patch_Strings::STRING_NEW_LINE;
    *this->stream << Patch_Strings::STRING_COMMENT << " " << Patch_Strings::STRING_CREATED + " " + QDate::currentDate().toString("dddd, MMMM dd, yyyy") + ", at " + QTime::currentTime().toString("hh:mm:ss A") + "." + Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Patch_Writer::Write_Next_Patch(const qint64 offset, const QString &value) {
    *this->stream << Patch_Strings::STRING_OFFSET << " " << Patch_Strings::STRING_HEX_IDENTIFIER
                  << this->valueManipulator->Convert_Offset_To_QString_With_Padding(offset, this->numDigitsInOffset)
                  << Patch_Strings::STRING_NEW_LINE;
    QString wrappedValue = this->valueManipulator->Wrap_QString_With_New_Lines(value, this->numDigitsInOffset);
    *this->stream << wrappedValue;
    if (!wrappedValue.endsWith(Patch_Strings::STRING_NEW_LINE)) *this->stream << Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Patch_Writer::Write_Next_Patch(const qint64 offset, const QByteArray &value) {
    return this->Write_Next_Patch(offset, this->valueManipulator->Convert_QByteArray_To_QString(value));
}
