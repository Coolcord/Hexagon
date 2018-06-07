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
    Hexagon();
    void Startup(QWidget *parent, const QString &location);
    void Shutdown() {}

    //Standard Functions
    Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation, const QString &outputFileLocation, int &lineNum);
    Hexagon_Error_Codes::Error_Code Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation, const QString &outputFileLocation, int &lineNume);

    //Dev Tools
    Hexagon_Error_Codes::Error_Code Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation, int &lineNum);
    Hexagon_Error_Codes::Error_Code Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation, int &lineNum);
    Hexagon_Error_Codes::Error_Code Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations,
                                                                                const QStringList &conflictList, int &lineNum, int &otherLineNum, int &otherFileNum);

    //Settings
    bool Set_Compare_Size(int size);

private:
    Hexagon(const Hexagon&);
    Hexagon& operator=(const Hexagon&);
    QString applicationLocation;
    QWidget *parent;
    int compareSize;
};

#endif // HEXAGON_H
