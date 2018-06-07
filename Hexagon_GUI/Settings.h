#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

struct Settings {
    QString defaultPatchOpenLocation;
    QString defaultFileOpenLocation;
    QString originalFileLocation;
    int compareSize;
    bool askForSaveLocation;
};

#endif // SETTINGS_H
