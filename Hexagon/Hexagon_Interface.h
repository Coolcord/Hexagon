#ifndef HEXAGON_INTERFACE_H
#define HEXAGON_INTERFACE_H

#include <QByteArray>
#include <QFile>
#include <QObject>
#include <QtPlugin>
#include <QString>
#include "Hexagon_Error_Codes.h"

class Hexagon_Interface : public QObject {
public:
    //Standard Functions
    virtual Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation,
                                                                const QString &outputFileLocation, bool useChecksum, int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QByteArray &patchFileBytes, QFile *outputFile, bool useChecksum, int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation,
                                                                 const QString &outputFileLocation, int compareSize, bool useChecksum, bool allowSizeDifference)=0;

    //Manually create patch
    virtual Hexagon_Error_Codes::Error_Code Start_Creating_Patch(const QString &outputPatchLocation)=0;
    virtual Hexagon_Error_Codes::Error_Code Start_Creating_Patch(const QString &outputPatchLocation, const QString &checksum)=0;
    virtual Hexagon_Error_Codes::Error_Code Start_Creating_Patch(const QString &outputPatchLocation, qint64 size)=0;
    virtual Hexagon_Error_Codes::Error_Code Start_Creating_Patch(const QString &outputPatchLocation, const QString &checksum, qint64 size)=0;
    virtual Hexagon_Error_Codes::Error_Code Cancel_Creating_Patch()=0;
    virtual Hexagon_Error_Codes::Error_Code Write_Next_Patch(qint64 offset, const QByteArray &bytes)=0;
    virtual Hexagon_Error_Codes::Error_Code Finish_Creating_Patch()=0;

    //Dev Tools
    virtual Hexagon_Error_Codes::Error_Code Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation,
                                                                             int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation,
                                                                             int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Check_For_Compatibility_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                            QString &output, int &lineNum, int &otherLineNum, int &otherFileNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                        QString &output, int &lineNum, int &otherLineNum, int &otherFileNum, bool verbose)=0;
    virtual bool Is_Line_End_Of_Header(const QString &line)=0;
};

Q_DECLARE_INTERFACE(Hexagon_Interface, "Hexagon_Interface")

#endif // HEXAGON_INTERFACE_H
