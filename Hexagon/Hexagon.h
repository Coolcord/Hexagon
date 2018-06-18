#ifndef HEXAGON_H
#define HEXAGON_H

#include "Hexagon_Interface.h"
#include <QFile>
#include <QTextStream>
#include <QString>

class Hexagon : public Hexagon_Interface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "Hexagon_Interface")
    Q_INTERFACES(Hexagon_Interface)

public:
    Hexagon() {}

    //Standard Functions
    Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation,
                                                        const QString &outputFileLocation, bool useChecksum, int &lineNum);
    Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QByteArray &patchFileBytes, QFile *outputFile, bool useChecksum, int &lineNum);
    Hexagon_Error_Codes::Error_Code Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation,
                                                         const QString &outputFileLocation, int compareSize, bool useChecksum, bool useComments);

    //Dev Tools
    Hexagon_Error_Codes::Error_Code Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation, int &lineNum);
    Hexagon_Error_Codes::Error_Code Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation, int &lineNum, bool useComments);
    Hexagon_Error_Codes::Error_Code Check_For_Compatibility_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                    QString &output, int &lineNum, int &otherLineNum, int &otherFileNum);
    Hexagon_Error_Codes::Error_Code Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                QString &output, int &lineNum, int &otherLineNum, int &otherFileNum, bool verbose);

private:
    Hexagon(const Hexagon&);
    Hexagon& operator=(const Hexagon&);
    Hexagon_Error_Codes::Error_Code Check_For_Conflicts(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                        QString &output, int &lineNum, int &otherLineNum, int &otherFileNum, bool verbose, bool conflicts);
};

#endif // HEXAGON_H
