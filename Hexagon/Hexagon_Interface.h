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
    virtual void Startup(QWidget *parent, const QString &location)=0;
    virtual void Shutdown()=0;

    //Standard Functions
    virtual Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation,
                                                                const QString &outputFileLocation, bool useChecksum, int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QByteArray &patchFileBytes, QFile *outputFile, bool useChecksum, int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation,
                                                                 const QString &outputFileLocation, int compareSize, bool useChecksum, bool useComments)=0;

    //Dev Tools
    virtual Hexagon_Error_Codes::Error_Code Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation,
                                                                             int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation,
                                                                             int &lineNum, bool useComments)=0;
    virtual Hexagon_Error_Codes::Error_Code Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                        QString &output, int &lineNum, int &otherLineNum, int &otherFileNum, bool verbose)=0;
};

Q_DECLARE_INTERFACE(Hexagon_Interface, "Hexagon_Interface")

#endif // HEXAGON_INTERFACE_H
