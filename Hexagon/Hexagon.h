#ifndef HEXAGON_H
#define HEXAGON_H

#include "Hexagon_Interface.h"
#include <QFile>
#include <QTextStream>
#include <QString>

class Patch_Writer;
class Value_Manipulator;

class Hexagon : public Hexagon_Interface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "Hexagon_Interface")
    Q_INTERFACES(Hexagon_Interface)

public:
    Hexagon();
    ~Hexagon();

    //Standard Functions
    Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation,
                                                        const QString &outputFileLocation, bool useChecksum, int &lineNum);
    Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QByteArray &patchFileBytes, QFile *outputFile, bool useChecksum, int &lineNum);
    Hexagon_Error_Codes::Error_Code Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation,
                                                         const QString &outputFileLocation, int compareSize, bool useChecksum, bool allowSizeDifference);

    //Manually create patch
    Hexagon_Error_Codes::Error_Code Start_Creating_Patch(const QString &outputPatchLocation);
    Hexagon_Error_Codes::Error_Code Start_Creating_Patch(const QString &outputPatchLocation, const QString &checksum);
    Hexagon_Error_Codes::Error_Code Start_Creating_Patch(const QString &outputPatchLocation, qint64 size);
    Hexagon_Error_Codes::Error_Code Start_Creating_Patch(const QString &outputPatchLocation, const QString &checksum, qint64 size);
    Hexagon_Error_Codes::Error_Code Write_Next_Patch(qint64 offset, const QByteArray &bytes);
    Hexagon_Error_Codes::Error_Code Finish_Creating_Patch();

    //Dev Tools
    Hexagon_Error_Codes::Error_Code Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation, int &lineNum);
    Hexagon_Error_Codes::Error_Code Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation, int &lineNum);
    Hexagon_Error_Codes::Error_Code Check_For_Compatibility_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                    QString &output, int &lineNum, int &otherLineNum, int &otherFileNum);
    Hexagon_Error_Codes::Error_Code Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                QString &output, int &lineNum, int &otherLineNum, int &otherFileNum, bool verbose);
    bool Is_Line_End_Of_Header(const QString &line);

private:
    Hexagon(const Hexagon&);
    Hexagon& operator=(const Hexagon&);
    Hexagon_Error_Codes::Error_Code Check_For_Conflicts(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                        QString &output, int &lineNum, int &otherLineNum, int &otherFileNum, bool verbose, bool conflicts);

    QFile *manualPatchFile;
    Patch_Writer *manualPatchWriter;
    Value_Manipulator *manualPatchFileValueManipulator;
};

#endif // HEXAGON_H
