#ifndef PATCH_READER_H
#define PATCH_READER_H

#include <QByteArray>
#include <QFile>
#include <QTextStream>

class Patch_Reader {
public:
    Patch_Reader(QFile *file);
    Patch_Reader(const QByteArray &patchBytes);
    ~Patch_Reader();
    int Get_Current_Line_Num();
    bool Get_Checksum(QString &checksum);
    bool Get_Next_Offset_And_Value(qint64 &offset, QByteArray &value);

private:
    bool Convert_QString_To_QByteArray(const QString &string, QByteArray &output);
    QString Get_Next_Line_After_Comments();
    bool Is_Line_Hex_String(const QString &line);
    bool Parse_Value(QByteArray &value);
    QString Trim_Hex_Identifier(QString &hexString);

    QTextStream *stream;
    int currentLineNum;
};

#endif // PATCH_READER_H
