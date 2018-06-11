#include "Patch_Comparer.h"
#include "Patch_Reader.h"
#include <assert.h>

Patch_Comparer::Patch_Comparer(Value_Manipulator *valueManipulator) {
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->usedValues = new QVector<QMap<qint64, unsigned char>*>();
}

Patch_Comparer::~Patch_Comparer() {
    for (int i = 0; i < this->usedValues->size(); ++i) {
        delete this->usedValues->at(i);
        this->usedValues->replace(i, NULL);
    }
    delete this->usedValues;
}

bool Patch_Comparer::Open_Original_File(const QString &fileLocation, int &lineNum) {
    return this->Open_File(fileLocation, lineNum, true);
}

bool Patch_Comparer::Open_Additional_File(const QString &fileLocation, int &lineNum) {
    return this->Open_File(fileLocation, lineNum, false);
}

bool Patch_Comparer::Open_File(const QString &fileLocation, int &lineNum, bool isOriginal) {
    QFile file(fileLocation);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return false;
    Patch_Reader patchReader(&file, this->valueManipulator);

    //Read the checksum just to get to the values
    QString checksum;
    if (!patchReader.Get_Checksum(checksum)) return false;

    int index = 0;
    if (isOriginal && !this->usedValues->isEmpty()) {
        this->usedValues->replace(0, new QMap<qint64, unsigned char>());
        index = 0;
    } else {
        this->usedValues->push_back(new QMap<qint64, unsigned char>());
        index = this->usedValues->size()-1;
    }

    //Insert all of the values of the patch into the map
    bool parseError = false;
    qint64 offset = 0;
    QByteArray value;
    while (patchReader.Get_Next_Offset_And_Value(offset, value, parseError)) {
        for (qint64 i = 0; i < value.size(); ++i) {
            this->usedValues->at(index)->insert(offset+i, static_cast<unsigned char>(value.at(i)));
        }
    }

    //Delete the map if it is invalid
    if (parseError) {
        lineNum = patchReader.Get_Current_Line_Num();
        if (isOriginal) {
            delete this->usedValues->at(0);
        } else {
            delete this->usedValues->last();
            this->usedValues->pop_back();
        }
    }
    return parseError;
}
