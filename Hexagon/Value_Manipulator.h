#ifndef VALUE_MANIPULATOR_H
#define VALUE_MANIPULATOR_H

#include <QByteArray>
#include <QFile>
#include <QString>

class Value_Manipulator {
public:
    Value_Manipulator() {}
    ~Value_Manipulator() {}
    QString Convert_QByteArray_To_QString(const QByteArray &bytes);
    QString Convert_Offset_To_QString_With_Padding(qint64 offset, int numberOfDigits);
    bool Convert_QString_To_QByteArray(const QString &string, QByteArray &output);
    QString Get_Checksum_From_File(QFile *file);
    bool Is_Line_Hex_String(const QString &line);
    QString Trim_Hex_Identifier(QString &hexString);
    QString Wrap_QString_With_New_Lines(const QString &string, int width);

private:
    char Get_Char_From_Nibble(unsigned char nibble);
};

#endif // VALUE_MANIPULATOR_H
