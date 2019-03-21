#ifndef PATCH_WRITER_H
#define PATCH_WRITER_H

#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QString>

class Value_Manipulator;

class Patch_Writer {
public:
    Patch_Writer(QFile *file, Value_Manipulator *valueManipulator);
    Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, const QString &originalFileName);
    Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, int numDigitsInOffset);
    Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, int numDigitsInOffset, const QString &originalFileName);
    ~Patch_Writer();
    bool Write_Break_Line();
    bool Write_Checksum(const QString &checksum);
    bool Write_Size(qint64 sizeDifference);
    bool Write_Next_Patch(const qint64 offset, const QString &value);
    bool Write_Next_Patch(const qint64 offset, const QByteArray &value);

private:
    void Initialize(QFile *file, Value_Manipulator *valueManipulator, int numDigitsInOffset, const QString &originalFileName);

    QTextStream *stream;
    Value_Manipulator *valueManipulator;
    QString breakString;
    QString originalFileName;
    int numDigitsInOffset;
};

#endif // PATCH_WRITER_H
