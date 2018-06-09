#include "Parser.h"
#include "Patch_Strings.h"

Parser::Parser(const QByteArray &patchBytes) {
    this->stream = new QTextStream(patchBytes, QIODevice::ReadOnly);
}

Parser::~Parser() {
    delete this->stream;
}

int Parser::Get_Current_Line_Num() {
    return this->currentLineNum;
}

bool Parser::Get_Checksum(QString &checksum) {
    QString line = this->Get_Next_Line_After_Comments();
    if (!line.startsWith(Patch_Strings::STRING_CHECKSUM)) return false;
    QStringList values = line.split(' ');
    if (values.size() != 2) return false;
    checksum = values.at(1);
    return true;
}

bool Parser::Get_Next_Offset_And_Value(qint64 offset, QByteArray &value) {
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
    line = this->Get_Next_Line_After_Comments();
    if (!line.startsWith(Patch_Strings::STRING_VALUE)) return false;
    values = line.split(' ');
    if (values.size() != 2) return false;
    QString valueString = values.at(1);
    valueString = this->Trim_Hex_Identifier(valueString);
    //TODO: Write this...

}

QString Parser::Get_Next_Line_After_Comments() {
    QString line = QString();
    do {
        ++this->currentLineNum;
        line = this->stream->readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.startsWith(Patch_Strings::STRING_COMMENT)) continue;
        else return line;
    } while (!this->stream->atEnd());
    return QString(); //nothing left to read
}

QString Parser::Trim_Hex_Identifier(QString &hexString) {
    if (hexString.startsWith(Patch_Strings::STRING_HEX_IDENTIFIER)) {
        int hexIdentifierSize = Patch_Strings::STRING_HEX_IDENTIFIER.size();
        return hexString.remove(hexIdentifierSize, hexString.size()-hexIdentifierSize);
    }
    return hexString;
}
