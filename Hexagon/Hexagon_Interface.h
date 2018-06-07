#ifndef HEXAGON_INTERFACE_H
#define HEXAGON_INTERFACE_H

#include <QObject>
#include <QtPlugin>
#include <QString>

namespace Hexagon_Error_Codes {
    enum Error_Code {
        OK,
        READ_ERROR,
        WRITE_ERROR,
        PARSE_ERROR,
        CONFLICTS_DETECTED
    };
}

class Hexagon_Interface : public QObject {
public:
    virtual void Startup(QWidget *parent, const QString &location)=0;
    virtual void Shutdown()=0;

    //Standard Functions
    virtual Hexagon_Error_Codes::Error_Code Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation,
                                                                const QString &outputFileLocation, int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation,
                                                                 const QString &outputFileLocation)=0;

    //Dev Tools
    virtual Hexagon_Error_Codes::Error_Code Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation,
                                                                             int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation,
                                                                             int &lineNum)=0;
    virtual Hexagon_Error_Codes::Error_Code Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation,
                                                                                        const QStringList &otherPatchFileLocations,
                                                                                        const QStringList &conflictList,
                                                                                        int &lineNum, int &otherLineNum, int &otherFileNum)=0;

    //Settings
    virtual bool Set_Compare_Size(int size)=0;
};

Q_DECLARE_INTERFACE(Hexagon_Interface, "Hexagon_Interface")

#endif // HEXAGON_INTERFACE_H
