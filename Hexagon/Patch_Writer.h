#ifndef PATCH_WRITER_H
#define PATCH_WRITER_H

#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QString>

class Value_Manipulator;

class Patch_Writer {
public:
    Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, bool useComments);
    Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, bool useComments, const QString &originalFileName);
    Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, bool useComments, int numDigitsInOffset);
    Patch_Writer(QFile *file, Value_Manipulator *valueManipulator, bool useComments, int numDigitsInOffset, const QString &originalFileName);
    ~Patch_Writer();
    bool Write_Break_Line();
    bool Write_Checksum(const QString &checksum);
    bool Write_Comment(const QString &comment);
    bool Write_Header();
    bool Write_Next_Patch(const qint64 offset, const QString &value);
    bool Write_Next_Patch(const qint64 offset, const QByteArray &value);

private:
    void Initialize(QFile *file, Value_Manipulator *valueManipulator, bool useComments, int numDigitsInOffset, const QString &originalFileName);

    QTextStream *stream;
    Value_Manipulator *valueManipulator;
    QString breakString;
    QString originalFileName;
    int numDigitsInOffset;
    bool useComments;
};

#endif // PATCH_WRITER_H
