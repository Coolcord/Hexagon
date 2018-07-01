#include "Hexagon.h"
#include "File_Comparer.h"
#include "File_Writer.h"
#include "Patch_Comparer.h"
#include "Patch_Reader.h"
#include "Patch_Strings.h"
#include "Patch_Writer.h"
#include "Qt_Code_Reader.h"
#include "Qt_Code_Writer.h"
#include "Value_Manipulator.h"
#include <assert.h>
#include <QFileInfo>

#include <QDebug>
#include <QElapsedTimer>

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
    if (!QFile::copy(originalFileLocation, outputFileLocation) || !outputFile.open(QIODevice::ReadWrite)) {
        outputFile.remove();
        return Hexagon_Error_Codes::WRITE_ERROR;
    }

    //Apply the patch
    Hexagon_Error_Codes::Error_Code errorCode = this->Apply_Hexagon_Patch(patchFileBytes, &outputFile, useChecksum, lineNum);
    outputFile.close();
    if (errorCode != Hexagon_Error_Codes::OK) outputFile.remove();
    return errorCode;
}

Hexagon_Error_Codes::Error_Code Hexagon::Apply_Hexagon_Patch(const QByteArray &patchFileBytes, QFile *outputFile, bool useChecksum, int &lineNum) {
    lineNum = 0;
    if (!outputFile) return Hexagon_Error_Codes::WRITE_ERROR;
    Value_Manipulator valueManipulator;
    Patch_Reader patchReader(patchFileBytes, &valueManipulator);

    //Read the checksum
    QString expectedChecksum = QString();
    QString actualChecksum = QString();
    if (!patchReader.Get_Checksum(expectedChecksum)) return Hexagon_Error_Codes::PARSE_ERROR;
    if (useChecksum) {
        actualChecksum = valueManipulator.Get_Checksum_From_File(outputFile);
        if (expectedChecksum != Patch_Strings::STRING_SKIP_CHECKSUM) {
            if (expectedChecksum != actualChecksum) {
                lineNum = patchReader.Get_Current_Line_Num();
                return Hexagon_Error_Codes::BAD_CHECKSUM;
            }
        }
    }

    qDebug() << "Starting run...";
    QElapsedTimer timer, readTimer, writeTimer;
    qint64 writeTime = 0, readTime = 0;
    timer.start();

    //Parse the patch file
    bool parseError = false, seekError = false;
    qint64 offset = 0;
    QByteArray value;
    File_Writer fileWriter(outputFile, &valueManipulator);
    readTimer.start();
    while (patchReader.Get_Next_Offset_And_Value(offset, value, parseError) && !parseError) {
        readTime += readTimer.elapsed();
        writeTimer.start();
        if (!fileWriter.Write_Bytes_To_Offset(offset, value, seekError)) {
            lineNum = patchReader.Get_Current_Line_Num();
            if (seekError) return Hexagon_Error_Codes::OFFSET_OUT_OF_RANGE;
            else return Hexagon_Error_Codes::WRITE_ERROR;
        }
        writeTime += writeTimer.elapsed();
        readTimer.start();
    }
    qDebug().noquote() << "Run finished in " << timer.elapsed() << " milliseconds";
    qDebug().noquote() << "Read Time:  " << readTime << " milliseconds";
    qDebug().noquote() << "Write Time: " << writeTime << " milliseconds";

    lineNum = patchReader.Get_Current_Line_Num();
    if (parseError) return Hexagon_Error_Codes::PARSE_ERROR;
    else return Hexagon_Error_Codes::OK;
}

Hexagon_Error_Codes::Error_Code Hexagon::Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation,
                                                              const QString &outputFileLocation, int compareSize, bool useChecksum, bool useComments) {
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
    Patch_Writer patchWriter(&outputFile, &valueManipulator, useComments, QString::number(originalFileInfo.size(), 0x10).size(), originalFileInfo.fileName());
    if (!patchWriter.Write_Header()) {
        outputFile.remove();
        return Hexagon_Error_Codes::WRITE_ERROR;
    }

    //Write the checksum
    if (!useChecksum) checksum = Patch_Strings::STRING_SKIP_CHECKSUM;
    if (!patchWriter.Write_Checksum(checksum) || !patchWriter.Write_Break_Line()) {
        outputFile.remove();
        return Hexagon_Error_Codes::WRITE_ERROR;
    }

    //Write the patches to the output file
    for (int i = 0; i < differences.size(); ++i) {
        QByteArray *bytes = differences.at(i).second;
        if (!bytes) continue;
        qint64 offset = differences.at(i).first;
        if (!patchWriter.Write_Next_Patch(offset, *bytes) || !patchWriter.Write_Break_Line()) {
            outputFile.remove();
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
    qint64 offset = 0, tmpOffset = 0;
    QByteArray value, tmpValue;
    bool parseError = false, doneReading = false, writeError = false;
    bool success = patchReader.Get_Next_Offset_And_Value(tmpOffset, tmpValue, parseError);
    do {
        offset = tmpOffset;
        value = tmpValue;
        success = patchReader.Get_Next_Offset_And_Value(tmpOffset, tmpValue, parseError);
        doneReading = !success && !parseError;
        if (doneReading) writeError = qtCodeWriter.Write_Last_Patch(offset, value);
        else writeError = qtCodeWriter.Write_Next_Patch(offset, value);
        if (!writeError) {
            lineNum = patchReader.Get_Current_Line_Num();
            patchFile.close();
            outputFile.remove();
            return Hexagon_Error_Codes::WRITE_ERROR;
        }

    } while (!doneReading && !parseError);
    patchFile.close();
    outputFile.close();
    lineNum = patchReader.Get_Current_Line_Num();
    if (parseError) {
        outputFile.remove();
        return Hexagon_Error_Codes::PARSE_ERROR;
    } else {
        return Hexagon_Error_Codes::OK;
    }
}

Hexagon_Error_Codes::Error_Code Hexagon::Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation, int &lineNum, bool useComments) {
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
    Patch_Writer patchWriter(&outputFile, &valueManipulator, useComments);
    if (!patchWriter.Write_Header() || !patchWriter.Write_Checksum(Patch_Strings::STRING_SKIP_CHECKSUM)
            || !patchWriter.Write_Break_Line()) {
        qtCodeFile.close();
        outputFile.remove();
        return Hexagon_Error_Codes::WRITE_ERROR;
    }

    //Read each patch from the Qt Code file and write it to the patch file
    qint64 offset = 0;
    QString value = QString();
    bool parseError = false;
    while (qtCodeReader.Read_Next_Patch(offset, value, parseError) && !parseError) {
        if (!patchWriter.Write_Next_Patch(offset, value) || !patchWriter.Write_Break_Line()) {
            qtCodeFile.close();
            outputFile.remove();
            lineNum = qtCodeReader.Get_Current_Line_Num();
            return Hexagon_Error_Codes::WRITE_ERROR;
        }
    }
    qtCodeFile.close();
    outputFile.close();
    lineNum = qtCodeReader.Get_Current_Line_Num();
    if (parseError) {
        outputFile.remove();
        return Hexagon_Error_Codes::PARSE_ERROR;
    } else {
        return Hexagon_Error_Codes::OK;
    }
}

Hexagon_Error_Codes::Error_Code Hexagon::Check_For_Compatibility_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                         QString &output, int &lineNum, int &otherLineNum, int &otherFileNum) {
    return this->Check_For_Conflicts(patchFileLocation, otherPatchFileLocations, output, lineNum, otherLineNum, otherFileNum, false, false);
}

Hexagon_Error_Codes::Error_Code Hexagon::Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                     QString &output, int &lineNum, int &otherLineNum, int &otherFileNum, bool verbose) {
    return this->Check_For_Conflicts(patchFileLocation, otherPatchFileLocations, output, lineNum, otherLineNum, otherFileNum, verbose, true);
}

Hexagon_Error_Codes::Error_Code Hexagon::Check_For_Conflicts(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                             QString &output, int &lineNum, int &otherLineNum, int &otherFileNum, bool verbose, bool conflicts) {
    lineNum = 0; otherLineNum = 0; otherFileNum = 0;
    Value_Manipulator valueManipulator;

    //Open the files into the Patch Comparer
    Patch_Comparer patchComparer(&valueManipulator);
    if (!patchComparer.Open_Original_File(patchFileLocation, lineNum)) {
        return Hexagon_Error_Codes::READ_ERROR;
    }
    lineNum = 0;
    for (int i = 0; i < otherPatchFileLocations.size(); ++i) {
        if (!patchComparer.Open_Additional_File(otherPatchFileLocations.at(i), otherLineNum)) {
            otherFileNum = i;
            return Hexagon_Error_Codes::READ_MODIFIED_ERROR;
        }
    }

    //Write all of the conflicts to a string
    QVector<QVector<qint64>*> conflictsAgainstAllFiles = patchComparer.Get_Conflicts();
    output = QString();
    bool findingsExist = false;
    QTextStream stream(&output);
    if (conflicts) {
        for (int i = 0; i < conflictsAgainstAllFiles.size(); ++i) {
            QVector<qint64> *conflictsAgainstFile = conflictsAgainstAllFiles.at(i);
            if (!conflictsAgainstFile) continue;
            if (!findingsExist) {
                findingsExist = true;
                QFileInfo patchFileInfo(patchFileLocation);
                stream << Patch_Strings::STRING_THE_FOLLOWING_PATCHES_ARE_INCOMPATIBLE_WITH << patchFileInfo.fileName() << ":" << Patch_Strings::STRING_NEW_LINE << Patch_Strings::STRING_NEW_LINE;
            }
            QFileInfo otherPatchFileInfo(otherPatchFileLocations.at(i));
            stream << otherPatchFileInfo.fileName() << Patch_Strings::STRING_NEW_LINE;
            if (verbose) {
                for (int j = 0; j < conflictsAgainstFile->size(); ++j) {
                    stream << Patch_Strings::STRING_HEX_IDENTIFIER << QString::number(conflictsAgainstFile->at(j), 0x10) << Patch_Strings::STRING_NEW_LINE;
                }
                stream << Patch_Strings::STRING_NEW_LINE;
            }
        }
    } else {
        for (int i = 0; i < conflictsAgainstAllFiles.size(); ++i) {
            QVector<qint64> *conflictsAgainstFile = conflictsAgainstAllFiles.at(i);
            if (!conflictsAgainstFile || conflictsAgainstFile->isEmpty()) {
                if (!findingsExist) {
                    findingsExist = true;
                    QFileInfo patchFileInfo(patchFileLocation);
                    stream << Patch_Strings::STRING_THE_FOLLOWING_PATCHES_ARE_COMPATIBLE_WITH << patchFileInfo.fileName() << ":" << Patch_Strings::STRING_NEW_LINE << Patch_Strings::STRING_NEW_LINE;
                }
                QFileInfo otherPatchFileInfo(otherPatchFileLocations.at(i));
                stream << otherPatchFileInfo.fileName() << Patch_Strings::STRING_NEW_LINE;
            }
        }
    }

    patchComparer.Deallocate_Findings(conflictsAgainstAllFiles);
    stream.flush();
    if (output.isEmpty()) return Hexagon_Error_Codes::OK;
    else return Hexagon_Error_Codes::FINDINGS_DETECTED;
}
