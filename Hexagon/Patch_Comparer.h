#ifndef PATCH_COMPARER_H
#define PATCH_COMPARER_H

#include <QMap>
#include <QVector>

class Value_Manipulator;

class Patch_Comparer {
public:
    Patch_Comparer(Value_Manipulator *valueManipulator);
    ~Patch_Comparer();
    bool Open_Original_File(const QString &fileLocation, int &lineNum);
    bool Open_Additional_File(const QString &fileLocation, int &lineNum);
    //QMap<qint64, unsigned char> Get_Conflicts(); //TODO: Don't return a map... you need something more efficient to iterate though...

private:
    bool Open_File(const QString &fileLocation, int &lineNum, bool isOriginal);

    QVector<QMap<qint64, unsigned char>*> *usedValues;
    Value_Manipulator *valueManipulator;
};

#endif // PATCH_COMPARER_H
