#ifndef SETTINGS_FILE_H
#define SETTINGS_FILE_H

#include "Settings.h"
#include <QString>

class Settings_File
{
public:
    Settings_File(const QString &applicationLocation);
    ~Settings_File() {}
    bool Save_Settings(const Settings &settings);
    bool Load_Settings(Settings &settings);
    void Load_Default_Settings(Settings &settings);

private:
    QString applicationLocation;
};

#endif // SETTINGS_FILE_H
