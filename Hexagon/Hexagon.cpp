#include "Hexagon.h"
#include "File_Writer.h"
#include "Patch_Reader.h"
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
    if (!outputFile.open(QIODevice::ReadWrite)) return Hexagon_Error_Codes::WRITE_ERROR;

    //Apply the patch
    Hexagon_Error_Codes::Error_Code errorCode = this->Apply_Hexagon_Patch(patchFileBytes, &outputFile, useChecksum, lineNum);
    outputFile.close();
    return errorCode;
}

Hexagon_Error_Codes::Error_Code Hexagon::Apply_Hexagon_Patch(const QByteArray &patchFileBytes, const QFile *outputFile, bool useChecksum, int &lineNum) {
    qDebug() << "Apply_Hexagon_Patch() called!";
    //TODO: Write this...
    return Hexagon_Error_Codes::OK;
}

Hexagon_Error_Codes::Error_Code Hexagon::Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation,
                                                              const QString &outputFileLocation, int compareSize, bool useChecksum, int &lineNume) {
    qDebug() << "Create_Hexagon_Patch() called!";
    //TODO: Write this...
    return Hexagon_Error_Codes::OK;
}

Hexagon_Error_Codes::Error_Code Hexagon::Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation, int &lineNum) {
    qDebug() << "Convert_Hexagon_Patch_To_Qt_Code() called!";
    //TODO: Write this...
    return Hexagon_Error_Codes::OK;
}

Hexagon_Error_Codes::Error_Code Hexagon::Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation, int &lineNum) {
    qDebug() << "Convert_Qt_Code_To_Hexagon_Patch() called!";
    //TODO: Write this...
    return Hexagon_Error_Codes::OK;
}

Hexagon_Error_Codes::Error_Code Hexagon::Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                     QByteArray &output, int &lineNum, int &otherLineNum, int &otherFileNum) {
    qDebug() << "Check_For_Conflicts_Between_Hexagon_Patches() called!";
    //TODO: Write this...
    output = output.append(QString("This is a test"));
    return Hexagon_Error_Codes::CONFLICTS_DETECTED;
}
