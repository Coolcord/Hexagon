#ifndef SETTINGS_FILE_H
#define SETTINGS_FILE_H

#include "Settings.h"

class Settings_File
{
public:
    Settings_File() {}
    ~Settings_File() {}
    bool Save_Settings(const Settings &settings);
    bool Load_Settings(Settings &settings);
    bool Load_Default_Settings(Settings &settings);
};

#endif // SETTINGS_FILE_H
