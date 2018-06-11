#ifndef FILE_COMPARER_H
#define FILE_COMPARER_H

#include <QString>
#include <QVector>
#include "Hexagon_Error_Codes.h"

class File_Comparer {
public:
    File_Comparer(QString originalFileLocation, QString modifiedFileLocation, int compareSize);
    ~File_Comparer() {}
    Hexagon_Error_Codes::Error_Code Scan_For_Differences(QVector<QPair<qint64, QByteArray*>> &differences);
    void Deallocate_Differences(QVector<QPair<qint64, QByteArray*>> &differences);

private:
    QString originalFileLocation;
    QString modifiedFileLocation;
    int compareSize;
};

#endif // FILE_COMPARER_H
