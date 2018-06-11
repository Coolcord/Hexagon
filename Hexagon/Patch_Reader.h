#ifndef PATCH_READER_H
#define PATCH_READER_H

#include <QByteArray>
#include <QFile>
#include <QTextStream>

class Value_Manipulator;

class Patch_Reader {
public:
    Patch_Reader(QFile *file, Value_Manipulator *valueManipulator);
    Patch_Reader(const QByteArray &patchBytes, Value_Manipulator *valueManipulator);
    ~Patch_Reader();
    int Get_Current_Line_Num();
    bool Get_Checksum(QString &checksum);
    bool Get_Next_Offset_And_Value(qint64 &offset, QByteArray &value, bool &parseError);

private:
    QString Get_Next_Line_After_Comments();
    bool Is_Line_Hex_String(const QString &line);
    bool Parse_Value(QByteArray &value);
    QString Trim_Hex_Identifier(QString &hexString);

    QTextStream *stream;
    Value_Manipulator *valueManipulator;
    int currentLineNum;
};

#endif // PATCH_READER_H
