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
    ~Patch_Writer();
    bool Write_Break_Line();
    bool Write_Checksum(const QString &checksum);
    bool Write_Comment(const QString &comment);
    bool Write_Header();
    bool Write_Next_Patch(const qint64 offset, const QString &value);
    bool Write_Next_Patch(const qint64 offset, const QByteArray &value);

private:
    QTextStream *stream;
    Value_Manipulator *valueManipulator;
    QString breakString;
    int numDigitsInOffset;
};

#endif // PATCH_WRITER_H
