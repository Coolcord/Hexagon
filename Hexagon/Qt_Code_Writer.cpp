#include "Qt_Code_Writer.h"
#include "Patch_Strings.h"
#include "Qt_Code_Strings.h"
#include "Value_Manipulator.h"
#include <assert.h>

const static int CODE_TEXT_WIDTH = 80;

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
    QString line = QString();
    if (lastPatch) line = Qt_Code_Strings::STRING_RETURN;
    else line = Qt_Code_Strings::STRING_IF_NOT;
    line += Qt_Code_Strings::STRING_WRITE_BYTES_TO_OFFSET+Patch_Strings::STRING_HEX_IDENTIFIER+QString::number(offset, 0x10);
    *this->stream << line;
    if (oneByte) {
        line = QString();
        *this->stream << Qt_Code_Strings::STRING_BYTE_ARRAY << "1" << Qt_Code_Strings::STRING_COMMA << Patch_Strings::STRING_HEX_IDENTIFIER
                      << value << Qt_Code_Strings::STRING_ENDING_PARENTHESES;
    } else {
        int currentLineSize = line.size();
        line = QString();
        *this->stream << Qt_Code_Strings::STRING_FROM_HEX;
        if (!this->Write_Value_Lines(value, currentLineSize)) return false;
        *this->stream << Qt_Code_Strings::STRING_TO_LATIN;
    }
    if (lastPatch) *this->stream << Qt_Code_Strings::STRING_SEMI_COLON << Patch_Strings::STRING_NEW_LINE;
    else *this->stream << Qt_Code_Strings::STRING_RETURN_FALSE << Patch_Strings::STRING_NEW_LINE;
    return this->stream->status() == QTextStream::Ok;
}

bool Qt_Code_Writer::Write_Value_Lines(const QString &value, int currentLineSize) {
    //Write the First Line
    *this->stream << "\"";
    int numChars = CODE_TEXT_WIDTH-currentLineSize;
    for (int i = 0; i < numChars && i < value.size(); ++i) {
        *this->stream << value.at(i);
    }

    //Write the Remaining Lines
    for (int i = numChars; i < value.size(); ++i) {
        if (i%CODE_TEXT_WIDTH == 0) *this->stream << "\"" << Patch_Strings::STRING_NEW_LINE << QString(QByteArray(8, ' ')) << "\"";
        *this->stream << value.at(i);
    }
    *this->stream << "\"";
    return this->stream->status() == QTextStream::Ok;
}
