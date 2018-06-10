#ifndef QT_CODE_WRITER_H
#define QT_CODE_WRITER_H

#include <QByteArray>
#include <QFile>
#include <QString>
#include <QTextStream>

class Qt_Code_Writer {
public:
    Qt_Code_Writer(QFile *file);
    ~Qt_Code_Writer();
    bool Write_Next_Patch(const qint64 offset, const QString &value);
    bool Write_Next_Patch(const qint64 offset, const QByteArray &value);

private:
    QTextStream *stream;
};

#endif // QT_CODE_WRITER_H
