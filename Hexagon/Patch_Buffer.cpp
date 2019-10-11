#include "Patch_Buffer.h"
#include "Patch_Writer.h"
#include "Value_Manipulator.h"

Patch_Buffer::Patch_Buffer(QFile *file) {
    this->valueManipulator = new Value_Manipulator();
    this->patchWriter = new Patch_Writer(file, this->valueManipulator);
    this->patchMap = new QMap<qint64, char>();
}

Patch_Buffer::~Patch_Buffer() {
    this->Flush();
    delete this->patchWriter;
    delete this->valueManipulator;
    delete this->patchMap;
}

void Patch_Buffer::Add_Next_Patch(const qint64 offset, const QByteArray &value) {
    for (int i = 0; i < value.size(); ++i) this->patchMap->insert(offset+i, value.at(i));
}

void Patch_Buffer::Clear_Buffer() {
    this->patchMap->clear();
}

bool Patch_Buffer::Flush() {
    if (this->patchMap->isEmpty()) return true; //nothing to do
    qint64 offset = 0;
    qint64 firstOffsetOfPatch = 0;
    qint64 previousOffset = 0;
    QByteArray bytes;
    for (QMap<qint64, char>::iterator iter = this->patchMap->begin(); iter != this->patchMap->end(); ++iter) {
        previousOffset = offset;
        offset = iter.key();
        char c = iter.value();
        if (offset == previousOffset+1) {
            bytes += c;
        } else {
            //Flush the current patch
            if (!this->Flush_Current_Buffer(firstOffsetOfPatch, bytes)) return false;
            bytes = QByteArray();
            bytes += c;
            firstOffsetOfPatch = offset;
        }
    }
    //Flush the remaining data
    if (!this->Flush_Current_Buffer(firstOffsetOfPatch, bytes)) return false;
    this->Clear_Buffer();
    return true;
}

bool Patch_Buffer::Write_Checksum(const QString &checksum) {
    return this->patchWriter->Write_Checksum(checksum);
}

bool Patch_Buffer::Write_Size(qint64 sizeDifference) {
    return this->patchWriter->Write_Size(sizeDifference);
}

bool Patch_Buffer::Flush_Current_Buffer(qint64 offset, const QByteArray &bytes) {
    if (!bytes.isEmpty()) {
        if (!this->patchWriter->Write_Next_Patch(offset, bytes)) return false;
        if (!this->patchWriter->Write_Break_Line()) return false;
    }
    return true;
}
