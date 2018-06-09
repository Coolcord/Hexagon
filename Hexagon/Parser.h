#ifndef PARSER_H
#define PARSER_H

#include <QByteArray>
#include <QTextStream>

class Parser {
public:
    Parser(const QByteArray &patchBytes);
    ~Parser();
    int Get_Current_Line_Num();
    bool Get_Checksum(QString &checksum);
    bool Get_Next_Offset_And_Value(qint64 offset, QByteArray &value);

private:
    QString Get_Next_Line_After_Comments();
    QString Trim_Hex_Identifier(QString &hexString);

    QTextStream *stream;
    int currentLineNum;
};

#endif // PARSER_H
