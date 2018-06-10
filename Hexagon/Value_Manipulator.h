#ifndef VALUE_MANIPULATOR_H
#define VALUE_MANIPULATOR_H

#include <QByteArray>
#include <QString>

class Value_Manipulator {
public:
    Value_Manipulator() {}
    ~Value_Manipulator() {}
    bool Convert_QByteArray_To_QString(const QByteArray &bytes, QString &output);
    QString Convert_Offset_To_QString_With_Padding(qint64 offset, int numberOfDigits);
    bool Convert_QString_To_QByteArray(const QString &string, QByteArray &output);
    bool Is_Line_Hex_String(const QString &line);
    QString Trim_Hex_Identifier(QString &hexString);
};

#endif // VALUE_MANIPULATOR_H
