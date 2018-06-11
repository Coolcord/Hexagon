#include "File_Comparer.h"
#include <assert.h>
#include <QFile>

const static qint64 BUFFER_SIZE = 51200; //50MB

File_Comparer::File_Comparer(QString originalFileLocation, QString modifiedFileLocation, int compareSize) {
    this->originalFileLocation = originalFileLocation;
    this->modifiedFileLocation = modifiedFileLocation;
    this->compareSize = compareSize;
}

Hexagon_Error_Codes::Error_Code File_Comparer::Scan_For_Differences(QVector<QPair<qint64, QByteArray*>> &differences) {
    differences.clear();
    QFile originalFile(this->originalFileLocation);
    QFile modifiedFile(this->modifiedFileLocation);
    if (originalFile.size() != modifiedFile.size()) return Hexagon_Error_Codes::SIZE_DIFFERENCE;
    if (!originalFile.exists() || !originalFile.open(QIODevice::ReadOnly)) return Hexagon_Error_Codes::READ_ERROR;
    if (!modifiedFile.exists() || !modifiedFile.open(QIODevice::ReadOnly)) {
        originalFile.close();
        return Hexagon_Error_Codes::READ_MODIFIED_ERROR;
    }

    //Scan both files for differences
    qint64 offset = 0;
    QByteArray *difference = NULL;
    int localCompareCount = 0;
    while (!originalFile.atEnd()) {
        QByteArray originalBytes = originalFile.read(BUFFER_SIZE);
        QByteArray modifiedBytes = modifiedFile.read(BUFFER_SIZE);
        assert(originalBytes.size() == modifiedBytes.size());

        //Search for differences in the buffer
        for (int i = 0; i < originalBytes.size(); ++i) {
            if (originalBytes.at(i) == modifiedBytes.at(i)) {
                assert(localCompareCount > 0);
                if (localCompareCount == 0) {
                    if (difference) {
                        differences.append(QPair<qint64, QByteArray*>(offset, difference));
                        difference = NULL;
                    }
                } else {
                    assert(difference);
                    difference->append(modifiedBytes.at(i));
                    --localCompareCount;
                }
            } else {
                if (originalBytes.at(i) != modifiedBytes.at(i)) { //difference found!
                    localCompareCount = this->compareSize;
                    if (!difference) difference = new QByteArray();
                    difference->append(modifiedBytes.at(i));
                }
            }
        }
    }

    //Append any remaining differences
    if (difference) {
        differences.append(QPair<qint64, QByteArray*>(offset, difference));
        difference = NULL;
    }
    originalFile.close();
    modifiedFile.close();
    return Hexagon_Error_Codes::OK;
}

void File_Comparer::Deallocate_Differences(QVector<QPair<qint64, QByteArray*>> &differences) {
    for (int i = 0; i < differences.size(); ++i) delete differences.at(i).second;
    differences.clear();
}
