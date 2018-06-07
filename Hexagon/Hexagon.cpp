#include "Hexagon.h"
#include <assert.h>

Hexagon::Hexagon() {
    this->applicationLocation = QString();
}

void Hexagon::Startup(QWidget *parent, const QString &location) {
    assert(parent);
    this->parent = parent;
    this->applicationLocation = location;
}

void Hexagon::Shutdown() {

}

bool Hexagon::Apply_Hexagon_Patch(const QString &patchFileLocation, const QString &originalFileLocation, const QString &outputFileLocation) {

}

bool Hexagon::Create_Hexagon_Patch(const QString &originalFileLocation, const QString &modifiedFileLocation, const QString &outputPatchLocation) {

}

bool Hexagon::Convert_Hexagon_Patch_To_Qt_Code(const QString &patchFileLocation, const QString &outputFileLocation) {

}

bool Hexagon::Convert_Qt_Code_To_Hexagon_Patch(const QString &qtCodeFileLocation, const QString &outputPatchLocation) {

}

bool Hexagon::Check_For_Conflicts_Between_Hexagon_Patches(const QString &patchFileLocation, const QStringList &otherPatchFileLocations) {

}

