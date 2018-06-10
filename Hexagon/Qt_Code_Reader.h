#ifndef QT_CODE_READER_H
#define QT_CODE_READER_H

#include <QByteArray>
#include <QFile>
#include <QString>
#include <QTextStream>

class Qt_Code_Reader {
public:
    Qt_Code_Reader(QFile *file);
    ~Qt_Code_Reader();
    bool Read_Next_Patch(qint64 &offset, QString &value);
    bool Read_Next_Patch(qint64 &offset, QByteArray &value);

private:
    QTextStream *stream;
    int currentLineNum;
};

#endif // QT_CODE_READER_H
