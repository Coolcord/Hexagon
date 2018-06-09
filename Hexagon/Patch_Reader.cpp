#include "Patch_Reader.h"
#include "Patch_Strings.h"
#include <assert.h>

Patch_Reader::Patch_Reader(const QByteArray &patchBytes) {
    this->stream = new QTextStream(patchBytes, QIODevice::ReadOnly);
}

Patch_Reader::~Patch_Reader() {
    delete this->stream;
}

int Patch_Reader::Get_Current_Line_Num() {
    return this->currentLineNum;
}

bool Patch_Reader::Get_Checksum(QString &checksum) {
    QString line = this->Get_Next_Line_After_Comments();
    if (!line.startsWith(Patch_Strings::STRING_CHECKSUM)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    checksum = values.at(1);
    return true;
}

bool Patch_Reader::Get_Next_Offset_And_Value(qint64 &offset, QByteArray &value) {
    //Get the Offset
    QString line = this->Get_Next_Line_After_Comments();
    if (!line.startsWith(Patch_Strings::STRING_OFFSET)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    QString offsetString = values.at(1);
    offsetString = this->Trim_Hex_Identifier(offsetString);
    bool valid = false;
    offset = offsetString.toULongLong(&valid, 0x10);
    if (!valid) return false;

    //Get the Value
    if (!this->Parse_Value(value)) return false;
    return true;
}

bool Patch_Reader::Convert_QString_To_QByteArray(const QString &string, QByteArray &output) {
    if (string.size()%2 == 1) return false;
    output = QByteArray(string.size()/2, 0x00);
    for (int i = 0, j = 0; i < string.size(); i += 2, ++j) {
        assert(j < output.size());
        QString numberString = QString(string.at(i))+QString(string.at(i+1));
        bool valid = false;
        int number = numberString.toInt(&valid, 0x10);
        if (!valid) return false;
        assert(numberString >= 0x00 && numberString <= 0xFF);
        output.data()[j] = static_cast<char>(number);
    }
    return true;
}

QString Patch_Reader::Get_Next_Line_After_Comments() {
    QString line = QString();
    while (!this->stream->atEnd()) {
        ++this->currentLineNum;
        line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.startsWith(Patch_Strings::STRING_COMMENT)) continue;
        else return line;
    }
    return QString(); //nothing left to read
}

bool Patch_Reader::Is_Line_Hex_String(const QString &line) {
    for (int i = 0; i < line.size(); ++i) {
        switch (line.at(i).toLower().toLatin1()) {
        default: return false;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
        case '8': case '9': case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': continue;
        }
    }
    return true;
}

bool Patch_Reader::Parse_Value(QByteArray &value) {
    //Parse the First Line
    QString line = this->Get_Next_Line_After_Comments();
    if (!line.startsWith(Patch_Strings::STRING_VALUE)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    QString valueString = values.at(1);
    valueString = this->Trim_Hex_Identifier(valueString);
    if (!this->Is_Line_Hex_String(valueString)) return false;

    //Parse Any Additional Lines
    while (!this->stream->atEnd()) {
        qint64 posBefore = this->stream->pos();
        line = this->stream->readLine().trimmed();
        if (line.startsWith(Patch_Strings::STRING_OFFSET)) {
            assert(this->stream->seek(posBefore));
            break;
        } else {
            ++this->currentLineNum;
            line = this->Trim_Hex_Identifier(line);
            if (!this->Is_Line_Hex_String(line)) return false;
            valueString += line;
        }
    }
    return this->Convert_QString_To_QByteArray(valueString, value);
}

QString Patch_Reader::Trim_Hex_Identifier(QString &hexString) {
    if (hexString.startsWith(Patch_Strings::STRING_HEX_IDENTIFIER)) {
        int hexIdentifierSize = Patch_Strings::STRING_HEX_IDENTIFIER.size();
        return hexString.remove(hexIdentifierSize, hexString.size()-hexIdentifierSize);
    }
    return hexString;
}
