#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings {
    QString defaultOriginalFileOpenLocation;
    QString defaultPatchOpenLocation;
    QString defaultFileOpenLocation;
    QString defaultPatchSaveLocation;
    QString defaultFileSaveLocation;
    QString originalFileLocation;
    int compareSize;
    bool alwaysAskForSaveLocation;
};

#endif // SETTINGS_H
