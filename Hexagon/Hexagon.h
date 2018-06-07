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
    void Shutdown();

    //Standard Functions
    bool Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation, const QString &outputFileLocation);
    bool Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation, const QString &outputPatchLocation);

    //Dev Tools
    bool Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation);
    bool Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputPatchLocation);
    bool Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations);

private:
    Hexagon(const Hexagon&);
    Hexagon& operator=(const Hexagon&);
    QString applicationLocation;
    QWidget *parent;
};

#endif // HEXAGON_H
