#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <QFile>

class Value_Manipulator;

class File_Writer {
public:
    File_Writer(QFile *file, Value_Manipulator *valueManipulator);
    ~File_Writer() {}
    bool Write_Bytes_To_Offset(qint64 offset, const QByteArray &bytes);
    bool Write_Bytes_To_Offset(qint64 offset, const QString &value);

private:
    QFile *file;
    Value_Manipulator *valueManipulator;
};

#endif // FILE_WRITER_H
