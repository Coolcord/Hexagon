#include "Patch_Writer.h"
#include "Patch_Strings.h"
#include "Value_Manipulator.h"
#include <assert.h>
#include <QDate>

const int DEFAULT_OFFSET_SIZE = 4;
const int PATCH_TEXT_WIDTH = 75;

Patch_Writer::Patch_Writer(QFile *file, Value_Manipulator *valueManipulator) {
    this->Initialize(file, valueManipulator, DEFAULT_OFFSET_SIZE, QString());
}

Patch_Writer::Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, const QString &originalFileName) {
    this->Initialize(file, valueManipulator, DEFAULT_OFFSET_SIZE, originalFileName);
}

Patch_Writer::Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, int numDigitsInOffset) {
    this->Initialize(file, valueManipulator, numDigitsInOffset, QString());
}

Patch_Writer::Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, int numDigitsInOffset, const QString &originalFileName) {
    this->Initialize(file, valueManipulator, numDigitsInOffset, originalFileName);
}

Patch_Writer::~Patch_Writer() {
    this->stream->flush();
    delete this->stream;
}

bool Patch_Writer::Write_Break_Line() {
    *this->stream << Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Patch_Writer::Write_Checksum(const QString &checksum) {
    *this->stream << Patch_Strings::STRING_CHECKSUM << " " << checksum << Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Patch_Writer::Write_Next_Patch(const qint64 offset, const QString &value) {
    *this->stream << Patch_Strings::STRING_OFFSET << " " << Patch_Strings::STRING_HEX_IDENTIFIER
                  << this->valueManipulator->Convert_Offset_To_QString_With_Padding(offset, this->numDigitsInOffset)
                  << Patch_Strings::STRING_NEW_LINE << Patch_Strings::STRING_VALUE << " ";
    QString wrappedValue = this->valueManipulator->Wrap_QString_With_New_Lines(value, PATCH_TEXT_WIDTH);
    *this->stream << wrappedValue;
    if (!wrappedValue.endsWith(Patch_Strings::STRING_NEW_LINE)) *this->stream << Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Patch_Writer::Write_Next_Patch(const qint64 offset, const QByteArray &value) {
    return this->Write_Next_Patch(offset, this->valueManipulator->Convert_QByteArray_To_QString(value));
}

void Patch_Writer::Initialize(QFile *file, Value_Manipulator *valueManipulator, int numDigitsInOffset, const QString &originalFileName) {
    assert(file);
    assert(file->isOpen() && file->isWritable());
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->numDigitsInOffset = numDigitsInOffset;
    this->originalFileName = originalFileName;
    this->stream = new QTextStream(file);
}
