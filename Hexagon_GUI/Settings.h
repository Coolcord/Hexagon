#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings {
    QString defaultOriginalFileOpenLocation;
    QString defaultPatchOpenLocation;
    QString defaultFileOpenLocation;
    QString defaultPatchSaveLocation;
    QString defaultFileSaveLocation;
    QString originalFileLocation1;
    QString originalFileLocation2;
    QString originalFileLocation3;
    int originalFileSlot;
    int compareSize;
    bool neverAskForSaveLocation;
    bool alwaysAskForSaveLocation;
    bool verboseConflictOutput;
    bool skipChecksumWhenCreatingPatch;
};

#endif // SETTINGS_H
