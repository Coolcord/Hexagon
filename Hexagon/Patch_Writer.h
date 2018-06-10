#ifndef PATCH_WRITER_H
#define PATCH_WRITER_H

#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QString>

class Patch_Writer {
public:
    Patch_Writer(QFile *file);
    ~Patch_Writer();
    bool Write_Break_Line();
    bool Write_Checksum(const QString &checksum);
    bool Write_Comment(const QString &comment);
    bool Write_Header();
    bool Write_Next_Patch(const qint64 offset, const QString &value);
    bool Write_Next_Patch(const qint64 offset, const QByteArray &value);

private:
    QTextStream *stream;
};

#endif // PATCH_WRITER_H
