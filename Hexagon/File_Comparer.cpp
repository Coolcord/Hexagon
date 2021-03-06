#include "File_Comparer.h"
#include "Value_Manipulator.h"
#include <assert.h>

const static qint64 BUFFER_SIZE = 51200; //50MB

File_Comparer::File_Comparer(const QString &originalFileLocation, const QString &modifiedFileLocation, int compareSize, Value_Manipulator *valueManipulator) {
    assert(valueManipulator);
    this->valueManipulator = valueManipulator;
    this->originalFileLocation = originalFileLocation;
    this->modifiedFileLocation = modifiedFileLocation;
    this->compareSize = compareSize;
}

Hexagon_Error_Codes::Error_Code File_Comparer::Scan_For_Differences(QVector<QPair<qint64, QByteArray*>> &differences, QString &originalChecksum) {
    differences.clear();
    QFile originalFile(this->originalFileLocation);
    QFile modifiedFile(this->modifiedFileLocation);
    if (!originalFile.exists() || !originalFile.open(QIODevice::ReadOnly)) return Hexagon_Error_Codes::READ_ERROR;
    if (!modifiedFile.exists() || !modifiedFile.open(QIODevice::ReadOnly)) {
        originalFile.close();
        return Hexagon_Error_Codes::READ_MODIFIED_ERROR;
    }

    //Get the checksum of the original file first
    originalChecksum = this->valueManipulator->Get_Checksum_From_File(&originalFile);
    if (originalChecksum.isEmpty()) return Hexagon_Error_Codes::READ_ERROR;

    //Scan both files for differences
    qint64 offset = 0;
    qint64 currentAddress = 0;
    qint64 originalFileSize = originalFile.size();
    QByteArray *difference = nullptr;
    int localCompareCount = 0;
    while (!modifiedFile.atEnd()) {
        QByteArray originalBytes = originalFile.read(BUFFER_SIZE);
        QByteArray modifiedBytes = modifiedFile.read(BUFFER_SIZE);

        //Search for differences in the buffer
        QByteArray tmpBuffer;
        for (int i = 0; i < modifiedBytes.size(); ++i, ++currentAddress) {
            if (currentAddress < originalFileSize && originalBytes.at(i) == modifiedBytes.at(i)) {
                assert(localCompareCount >= 0);
                if (localCompareCount == 0) {
                    if (difference) {
                        differences.append(QPair<qint64, QByteArray*>(offset, difference));
                        difference = nullptr;
                        tmpBuffer.clear();
                    }
                } else {
                    tmpBuffer.append(modifiedBytes.at(i));
                    --localCompareCount;
                }
            } else {
                if (currentAddress >= originalFileSize || originalBytes.at(i) != modifiedBytes.at(i)) { //difference found!
                    localCompareCount = this->compareSize-1;
                    if (!difference) {
                        offset = currentAddress;
                        difference = new QByteArray();
                    }
                    if (!tmpBuffer.isEmpty()) {
                        difference->append(tmpBuffer);
                        tmpBuffer.clear();
                    }
                    difference->append(modifiedBytes.at(i));
                }
            }
        }
    }

    //Append any remaining differences
    if (difference) {
        differences.append(QPair<qint64, QByteArray*>(offset, difference));
        difference = nullptr;
    }
    originalFile.close();
    modifiedFile.close();
    return Hexagon_Error_Codes::OK;
}

void File_Comparer::Deallocate_Differences(QVector<QPair<qint64, QByteArray*>> &differences) {
    for (int i = 0; i < differences.size(); ++i) delete differences.at(i).second;
    differences.clear();
}
