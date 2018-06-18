#ifndef PATCH_COMPARER_H
#define PATCH_COMPARER_H

#include <QVector>
#include <QMap>

class Value_Manipulator;

class Patch_Comparer {
public:
    Patch_Comparer(Value_Manipulator *valueManipulator);
    ~Patch_Comparer();
    bool Open_Original_File(const QString &fileLocation, int &lineNum);
    bool Open_Additional_File(const QString &fileLocation, int &lineNum);
    QVector<QVector<qint64>*> Get_Conflicts(); //the returned structure needs to be manually deallocated
    void Deallocate_Findings(QVector<QVector<qint64>*> &conflicts);

private:
    bool Open_File(const QString &fileLocation, int &lineNum, bool isOriginal);

    QVector<QPair<qint64, unsigned char>> *originalValues;
    QVector<QMap<qint64, unsigned char>*> *otherValues;
    Value_Manipulator *valueManipulator;
};

#endif // PATCH_COMPARER_H
