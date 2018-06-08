#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings {
    QString defaultPatchOpenLocation;
    QString defaultOriginalFileOpenLocation;
    QString defaultFileOpenLocation;
    QString originalFileLocation;
    int compareSize;
    bool alwaysAskForSaveLocation;
};

#endif // SETTINGS_H
