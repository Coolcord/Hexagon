#include "Hexagon.h"
#include <assert.h>
#include <QDebug>

const int DEFAULT_COMPARE_SIZE = 5;

Hexagon::Hexagon() {
    this->applicationLocation = QString();
}

void Hexagon::Startup(QWidget *parent, const QString &location) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = location;
    this->compareSize = DEFAULT_COMPARE_SIZE;
}

Hexagon_Error_Codes::Error_Code Hexagon::Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation, const QString &outputFileLocation, int &lineNum) {
    qDebug() << "Apply_Hexagon_Patch() called!";
    //TODO: Write this...
}

Hexagon_Error_Codes::Error_Code Hexagon::Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation, const QString &outputFileLocation) {
    qDebug() << "Create_Hexagon_Patch() called!";
    //TODO: Write this...
}

Hexagon_Error_Codes::Error_Code Hexagon::Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation, int &lineNum) {
    qDebug() << "Convert_Hexagon_Patch_To_Qt_Code() called!";
    //TODO: Write this...
}

Hexagon_Error_Codes::Error_Code Hexagon::Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation, int &lineNum) {
    qDebug() << "Convert_Qt_Code_To_Hexagon_Patch() called!";
    //TODO: Write this...
}

Hexagon_Error_Codes::Error_Code Hexagon::Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations, const QStringList &conflictList, int &lineNum, int &otherLineNum, int &otherFileNum) {
    qDebug() << "Check_For_Conflicts_Between_Hexagon_Patches() called!";
    //TODO: Write this...
}

bool Hexagon::Set_Compare_Size(int size) {
    if (compareSize <= 0) return false;
    this->compareSize = size;
    return true;
}

