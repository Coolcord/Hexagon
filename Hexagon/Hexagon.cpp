#include "Hexagon.h"
#include "File_Comparer.h"
#include "File_Writer.h"
#include "Patch_Reader.h"
#include "Patch_Strings.h"
#include "Patch_Writer.h"
#include "Qt_Code_Reader.h"
#include "Qt_Code_Writer.h"
#include "Value_Manipulator.h"
#include <assert.h>
#include <QFileInfo>

#include <QDebug>

Hexagon::Hexagon() {
    this->applicationLocation = QString();
}

void Hexagon::Startup(QWidget *parent, const QString &applicationLocation) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = applicationLocation;
}

Hexagon_Error_Codes::Error_Code Hexagon::Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation,
                                                             const QString &outputFileLocation, bool useChecksum, int &lineNum) {
    if (patchFileLocation.isEmpty()) return Hexagon_Error_Codes::READ_PATCH_ERROR;
    if (originalFileLocation.isEmpty()) return Hexagon_Error_Codes::READ_ERROR;
    if (outputFileLocation.isEmpty()) return Hexagon_Error_Codes::WRITE_ERROR;

    //Read the patch file into memory
    QFile patchFile(patchFileLocation);
    if (!patchFile.exists() || !patchFile.open(QIODevice::ReadOnly)) return Hexagon_Error_Codes::READ_PATCH_ERROR;
    QByteArray patchFileBytes = patchFile.readAll();
    if (patchFileBytes.size() == 0) return Hexagon_Error_Codes::READ_PATCH_ERROR;
    patchFile.close();

    //Copy the original file to the output location
    QFileInfo originalFileInfo(originalFileLocation);
    if (!originalFileInfo.exists() || !originalFileInfo.isReadable()) return Hexagon_Error_Codes::READ_ERROR;
    QFile outputFile(outputFileLocation);
    if (outputFile.exists() && !outputFile.remove()) return Hexagon_Error_Codes::WRITE_ERROR;
    if (!QFile::copy(originalFileLocation, outputFileLocation)) return Hexagon_Error_Codes::WRITE_ERROR;

    //Apply the patch
    Hexagon_Error_Codes::Error_Code errorCode = this->Apply_Hexagon_Patch(patchFileBytes, &outputFile, useChecksum, lineNum);
    outputFile.close();
    return errorCode;
}

Hexagon_Error_Codes::Error_Code Hexagon::Apply_Hexagon_Patch(const QByteArray &patchFileBytes, QFile *outputFile, bool useChecksum, int &lineNum) {
    lineNum = 0;
    if (!outputFile) return Hexagon_Error_Codes::WRITE_ERROR;
    Value_Manipulator valueManipulator;
    Patch_Reader patchReader(patchFileBytes, &valueManipulator);

    //Read the checksum
    QString expectedChecksum = QString();
    if (!patchReader.Get_Checksum(expectedChecksum)) return Hexagon_Error_Codes::PARSE_ERROR;
    QString actualChecksum = QString();
    if (!outputFile->open(QIODevice::ReadWrite)) return Hexagon_Error_Codes::WRITE_ERROR;
    if (useChecksum) {
        actualChecksum = valueManipulator.Get_Checksum_From_File(outputFile);
        if (actualChecksum != Patch_Strings::STRING_SKIP_CHECKSUM) {
            if (expectedChecksum != actualChecksum) {
                outputFile->close();
                lineNum = patchReader.Get_Current_Line_Num();
                return Hexagon_Error_Codes::BAD_CHECKSUM;
            }
        }
    }

    //Parse the patch file
    bool parseError = false, seekError = false;
    qint64 offset = 0;
    QByteArray value;
    File_Writer fileWriter(outputFile, &valueManipulator);
    while (patchReader.Get_Next_Offset_And_Value(offset, value, parseError) && !parseError) {
        if (!fileWriter.Write_Bytes_To_Offset(offset, value, seekError)) {
            outputFile->close();
            lineNum = patchReader.Get_Current_Line_Num();
            if (seekError) return Hexagon_Error_Codes::OFFSET_OUT_OF_RANGE;
            else return Hexagon_Error_Codes::WRITE_ERROR;
        }
    }
    outputFile->close();
    lineNum = patchReader.Get_Current_Line_Num();
    if (parseError) return Hexagon_Error_Codes::PARSE_ERROR;
    else return Hexagon_Error_Codes::OK;
}

Hexagon_Error_Codes::Error_Code Hexagon::Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation,
                                                              const QString &outputFileLocation, int compareSize, bool useChecksum) {
    if (originalFileLocation.isEmpty()) return Hexagon_Error_Codes::READ_ERROR;
    if (modifiedFileLocation.isEmpty()) return Hexagon_Error_Codes::READ_MODIFIED_ERROR;
    if (outputFileLocation.isEmpty()) return Hexagon_Error_Codes::WRITE_ERROR;
    Value_Manipulator valueManipulator;
    File_Comparer fileComparer(originalFileLocation, modifiedFileLocation, compareSize, &valueManipulator);

    //Run the scan for differences
    QVector<QPair<qint64, QByteArray*>> differences;
    QString checksum = QString();
    Hexagon_Error_Codes::Error_Code errorCode = fileComparer.Scan_For_Differences(differences, checksum);
    if (errorCode != Hexagon_Error_Codes::OK) {
        fileComparer.Deallocate_Differences(differences);
        return errorCode;
    }

    //Write the header
    QFileInfo originalFileInfo(originalFileLocation);
    QFile outputFile(outputFileLocation);
    if (outputFile.exists() && !outputFile.remove()) return Hexagon_Error_Codes::WRITE_ERROR;
    if (!outputFile.open(QIODevice::ReadWrite)) return Hexagon_Error_Codes::WRITE_ERROR;
    Patch_Writer patchWriter(&outputFile, &valueManipulator, QString::number(originalFileInfo.size(), 0x10).size(), originalFileInfo.fileName());
    if (!patchWriter.Write_Header()) {
        outputFile.close();
        return Hexagon_Error_Codes::WRITE_ERROR;
    }

    //Write the checksum
    if (!useChecksum) checksum = Patch_Strings::STRING_SKIP_CHECKSUM;
    if (!patchWriter.Write_Checksum(checksum) || !patchWriter.Write_Break_Line()) {
        outputFile.close();
        return Hexagon_Error_Codes::WRITE_ERROR;
    }

    //Write the patches to the output file
    for (int i = 0; i < differences.size(); ++i) {
        QByteArray *bytes = differences.at(i).second;
        if (!bytes) continue;
        qint64 offset = differences.at(i).first;
        if (!patchWriter.Write_Next_Patch(offset, *bytes) || !patchWriter.Write_Break_Line()) {
            outputFile.close();
            fileComparer.Deallocate_Differences(differences);
            return Hexagon_Error_Codes::WRITE_ERROR;
        }
    }
    fileComparer.Deallocate_Differences(differences);
    return Hexagon_Error_Codes::OK;
}

Hexagon_Error_Codes::Error_Code Hexagon::Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation, int &lineNum) {
    lineNum = 0;
    if (patchFileLocation.isEmpty()) return Hexagon_Error_Codes::READ_ERROR;
    if (outputFileLocation.isEmpty()) return Hexagon_Error_Codes::WRITE_ERROR;

    //Open the Patch File
    QFile patchFile(patchFileLocation);
    if (!patchFile.exists() || !patchFile.open(QIODevice::ReadOnly)) return Hexagon_Error_Codes::READ_ERROR;
    Value_Manipulator valueManipulator;
    Patch_Reader patchReader(&patchFile, &valueManipulator);

    //Read past the checksum
    QString checksum = QString();
    if (!patchReader.Get_Checksum(checksum)) {
        lineNum = patchReader.Get_Current_Line_Num();
        patchFile.close();
        return Hexagon_Error_Codes::READ_ERROR;
    }

    //Open the output file
    QFile outputFile(outputFileLocation);
    if (outputFile.exists() && !outputFile.remove()) return Hexagon_Error_Codes::WRITE_ERROR;
    if (!outputFile.open(QIODevice::ReadWrite)) return Hexagon_Error_Codes::WRITE_ERROR;
    Qt_Code_Writer qtCodeWriter(&outputFile, &valueManipulator);

    //Read each patch from the Patch file and write it to the Qt Code file
    qint64 offset = 0;
    QByteArray value;
    bool parseError = false, doneReading = false, writeError = false;
    bool success = patchReader.Get_Next_Offset_And_Value(offset, value, parseError);
    do {
        if (!success || parseError) break;
        doneReading = !success && !parseError;
        if (doneReading) writeError = qtCodeWriter.Write_Last_Patch(offset, value);
        else writeError = qtCodeWriter.Write_Next_Patch(offset, value);
        if (writeError) {
            lineNum = patchReader.Get_Current_Line_Num();
            patchFile.close();
            outputFile.close();
            return Hexagon_Error_Codes::WRITE_ERROR;
        }
        success = patchReader.Get_Next_Offset_And_Value(offset, value, parseError);
    } while (!doneReading && !parseError);
    patchFile.close();
    outputFile.close();
    lineNum = patchReader.Get_Current_Line_Num();
    if (parseError) return Hexagon_Error_Codes::PARSE_ERROR;
    else return Hexagon_Error_Codes::OK;
}

Hexagon_Error_Codes::Error_Code Hexagon::Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation, int &lineNum) {
    lineNum = 0;
    if (qtCodeFileLocation.isEmpty()) return Hexagon_Error_Codes::READ_ERROR;
    if (outputFileLocation.isEmpty()) return Hexagon_Error_Codes::WRITE_ERROR;

    //Open the Qt Code File
    QFile qtCodeFile(qtCodeFileLocation);
    if (!qtCodeFile.exists() || !qtCodeFile.open(QIODevice::ReadOnly)) return Hexagon_Error_Codes::READ_ERROR;
    Value_Manipulator valueManipulator;
    Qt_Code_Reader qtCodeReader(&qtCodeFile, &valueManipulator);

    //Write the header
    QFile outputFile(outputFileLocation);
    if (outputFile.exists() && !outputFile.remove()) return Hexagon_Error_Codes::WRITE_ERROR;
    if (!outputFile.open(QIODevice::ReadWrite)) return Hexagon_Error_Codes::WRITE_ERROR;
    Patch_Writer patchWriter(&outputFile, &valueManipulator);
    if (!patchWriter.Write_Header() || !patchWriter.Write_Checksum(Patch_Strings::STRING_SKIP_CHECKSUM)
            || !patchWriter.Write_Break_Line()) {
        qtCodeFile.close();
        outputFile.close();
        return Hexagon_Error_Codes::WRITE_ERROR;
    }

    //Read each patch from the Qt Code file and write it to the patch file
    qint64 offset = 0;
    QString value = QString();
    bool parseError = false;
    while (qtCodeReader.Read_Next_Patch(offset, value, parseError) && !parseError) {
        if (!patchWriter.Write_Next_Patch(offset, value) || !patchWriter.Write_Break_Line()) {
            qtCodeFile.close();
            outputFile.close();
            lineNum = qtCodeReader.Get_Current_Line_Num();
            return Hexagon_Error_Codes::WRITE_ERROR;
        }
    }
    qtCodeFile.close();
    outputFile.close();
    lineNum = qtCodeReader.Get_Current_Line_Num();
    if (parseError) return Hexagon_Error_Codes::PARSE_ERROR;
    else return Hexagon_Error_Codes::OK;
}

Hexagon_Error_Codes::Error_Code Hexagon::Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                     QByteArray &output, int &lineNum, int &otherLineNum, int &otherFileNum) {
    qDebug() << "Check_For_Conflicts_Between_Hexagon_Patches() called!";
    //TODO: Write this...
    output = output.append(QString("This is a test"));
    return Hexagon_Error_Codes::CONFLICTS_DETECTED;
}
