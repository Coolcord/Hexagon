#ifndef FILE_COMPARER_H
#define FILE_COMPARER_H

#include <QFile>
#include <QString>
#include <QVector>
#include "Hexagon_Error_Codes.h"

class Value_Manipulator;

class File_Comparer {
public:
    File_Comparer(const QString &originalFileLocation, const QString &modifiedFileLocation, int compareSize, Value_Manipulator *valueManipulator);
    ~File_Comparer() {}
    Hexagon_Error_Codes::Error_Code Scan_For_Differences(QVector<QPair<qint64, QByteArray*>> &differences, QString &originalChecksum);
    void Deallocate_Differences(QVector<QPair<qint64, QByteArray*>> &differences);

private:
    QString Get_Checksum(QFile *file);

    Value_Manipulator *valueManipulator;
    QString originalFileLocation;
    QString modifiedFileLocation;
    int compareSize;
};

#endif // FILE_COMPARER_H
