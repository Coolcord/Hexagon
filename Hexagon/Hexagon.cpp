#include "Hexagon.h"
#include <assert.h>

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

}

Hexagon_Error_Codes::Error_Code Hexagon::Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation, const QString &outputFileLocation) {

}

Hexagon_Error_Codes::Error_Code Hexagon::Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation, int &lineNum) {

}

Hexagon_Error_Codes::Error_Code Hexagon::Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputFileLocation, int &lineNum) {

}

Hexagon_Error_Codes::Error_Code Hexagon::Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations, const QStringList &conflictList, int &lineNum, int &otherLineNum, int &otherFileNum) {

}

bool Hexagon::Set_Compare_Size(int size) {
    if (compareSize <= 0) return false;
    this->compareSize = size;
    return true;
}

