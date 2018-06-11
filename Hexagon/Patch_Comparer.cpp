#include "Patch_Comparer.h"
#include "Patch_Reader.h"
#include <assert.h>

Patch_Comparer::Patch_Comparer(Value_Manipulator *valueManipulator) {
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->originalValues = new QVector<QPair<qint64, unsigned char>>();
    this->otherValues = new QVector<QMap<qint64, unsigned char>*>();
}

Patch_Comparer::~Patch_Comparer() {
    delete this->originalValues;
    for (int i = 0; i < this->otherValues->size(); ++i) {
        delete this->otherValues->at(i);
        this->otherValues->replace(i, NULL);
    }
    delete this->otherValues;
}

bool Patch_Comparer::Open_Original_File(const QString &fileLocation, int &lineNum) {
    return this->Open_File(fileLocation, lineNum, true);
}

bool Patch_Comparer::Open_Additional_File(const QString &fileLocation, int &lineNum) {
    return this->Open_File(fileLocation, lineNum, false);
}

QVector<QVector<qint64>*> Patch_Comparer::Get_Conflicts() {
    QVector<QVector<qint64>*> conflicts(this->otherValues->size());
    for (int i = 0; i < conflicts.size(); ++i) conflicts.replace(i, NULL);
    if (this->otherValues->size() < 2) return conflicts;

    //Start looking for conflicts
    for (int i = 0; i < this->otherValues->size(); ++i) {
        QMap<qint64, unsigned char> *map = this->otherValues->at(i);
        if (!map) continue;
        for (int j = 0; j < this->originalValues->size(); ++j) {
            qint64 offset = this->originalValues->at(j).first;
            QMap<qint64, unsigned char>::const_iterator iter = map->find(offset);
            if (iter == map->end()) continue; //no element found at that offset
            unsigned char value = iter.value();
            if (value != this->originalValues->at(j).second) { //conflict detected!
                if (!conflicts.at(i)) conflicts.replace(i, new QVector<qint64>());
                QVector<qint64> *vector = conflicts.at(i);
                vector->push_back(offset);
            }
        }
    }
    return conflicts;
}

void Patch_Comparer::Deallocate_Conflicts(QVector<QVector<qint64>*> &conflicts) {
    for (int i = 0; i < conflicts.size(); ++i) {
        delete conflicts.at(i);
        conflicts.replace(i, NULL);
    }
}

bool Patch_Comparer::Open_File(const QString &fileLocation, int &lineNum, bool isOriginal) {
    QFile file(fileLocation);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return false;
    Patch_Reader patchReader(&file, this->valueManipulator);

    //Read the checksum just to get to the values
    QString checksum;
    if (!patchReader.Get_Checksum(checksum)) return false;

    int index = 0;
    if (!isOriginal) {
        this->otherValues->push_back(new QMap<qint64, unsigned char>());
        index = this->otherValues->size()-1;
    }

    //Insert all of the values of the patch into the map
    bool parseError = false;
    qint64 offset = 0;
    QByteArray value;
    while (patchReader.Get_Next_Offset_And_Value(offset, value, parseError)) {
        for (qint64 i = 0; i < value.size(); ++i) {
            if (isOriginal) this->originalValues->push_back(QPair<qint64, unsigned char>(offset+i, static_cast<unsigned char>(value.at(i))));
            else this->otherValues->at(index)->insert(offset+i, static_cast<unsigned char>(value.at(i)));
        }
    }

    //Delete the map if it is invalid
    if (parseError) {
        lineNum = patchReader.Get_Current_Line_Num();
        if (isOriginal) {
            this->originalValues->clear();
        } else {
            delete this->otherValues->last();
            this->otherValues->pop_back();
        }
    }
    return parseError;
}
