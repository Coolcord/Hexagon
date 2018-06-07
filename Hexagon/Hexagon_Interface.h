#ifndef HEXAGON_INTERFACE_H
#define HEXAGON_INTERFACE_H

#include <QObject>
#include <QtPlugin>
#include <QString>

class Hexagon_Interface : public QObject {
public:
    virtual void Startup(QWidget *parent, const QString &location)=0;
    virtual void Shutdown()=0;

    //Standard Functions
    virtual bool Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation, const QString &outputFileLocation)=0;
    virtual bool Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation, const QString &outputPatchLocation)=0;

    //Dev Tools
    virtual bool Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation)=0;
    virtual bool Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputPatchLocation)=0;
    virtual bool Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations)=0;
};

Q_DECLARE_INTERFACE(Hexagon_Interface, "Hexagon_Interface")

#endif // HEXAGON_INTERFACE_H
