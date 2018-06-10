#ifndef QT_CODE_WRITER_H
#define QT_CODE_WRITER_H

#include <QByteArray>
#include <QFile>
#include <QString>
#include <QTextStream>

class Value_Manipulator;

class Qt_Code_Writer {
public:
    Qt_Code_Writer(QFile *file, Value_Manipulator *valueManipulator);
    ~Qt_Code_Writer();
    bool Write_Last_Patch(const qint64 offset, const QString &value);
    bool Write_Last_Patch(const qint64 offset, const QByteArray &value);
    bool Write_Next_Patch(const qint64 offset, const QString &value);
    bool Write_Next_Patch(const qint64 offset, const QByteArray &value);

private:
    bool Write_Patch(const qint64 offset, const QString &value, bool lastPatch);
    bool Write_Value_Lines(const QString &value);

    QTextStream *stream;
    Value_Manipulator *valueManipulator;
};

#endif // QT_CODE_WRITER_H
