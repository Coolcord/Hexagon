#ifndef QT_CODE_READER_H
#define QT_CODE_READER_H

#include <QByteArray>
#include <QFile>
#include <QString>
#include <QTextStream>

class Value_Manipulator;

class Qt_Code_Reader {
public:
    Qt_Code_Reader(QFile *file, Value_Manipulator *valueManipulator);
    ~Qt_Code_Reader();
    int Get_Current_Line_Num();
    bool Read_Next_Patch(qint64 &offset, QString &value, bool &parseError);
    bool Read_Next_Patch(qint64 &offset, QByteArray &value, bool &parseError);

private:
    QString Get_Next_Line_After_Comments();
    bool Get_Offset_From_Line(const QString &line, qint64 &offset);
    bool Get_Multi_Value_From_Lines(const QString &firstLine, QString &value);
    bool Get_Single_Value_From_Line(const QString &line, QString &value);

    QTextStream *stream;
    Value_Manipulator *valueManipulator;
    int currentLineNum;
};

#endif // QT_CODE_READER_H
