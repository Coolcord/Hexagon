#include "Settings_File.h"
#include "Common_Strings.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "../../C_Common_Code/Qt/Readable_Config_File/Readable_Config_File.h"

Settings_File::Settings_File(const QString &applicationLocation) {
    this->applicationLocation = applicationLocation;
    this->readableConfigFile = new Readable_Config_File();
}

Settings_File::~Settings_File() {
    delete this->readableConfigFile;
}

bool Settings_File::Save_Settings(const Settings &settings) {
    if (!this->readableConfigFile->Open_Without_Loading(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_HEXAGON_SETTINGS_FILENAME)) return false;
    if (!this->readableConfigFile->Set_Value("Default_Original_File_Open_Location", settings.defaultOriginalFileOpenLocation)) return false;
    if (!this->readableConfigFile->Set_Value("Default_Patch_Open_Location", settings.defaultPatchOpenLocation)) return false;
    if (!this->readableConfigFile->Set_Value("Default_File_Open_Location", settings.defaultFileOpenLocation)) return false;
    if (!this->readableConfigFile->Set_Value("Default_Patch_Save_Location", settings.defaultPatchSaveLocation)) return false;
    if (!this->readableConfigFile->Set_Value("Default_File_Save_Location", settings.defaultFileSaveLocation)) return false;
    if (!this->readableConfigFile->Set_Value("Original_File_Location_1", settings.originalFileLocation1)) return false;
    if (!this->readableConfigFile->Set_Value("Original_File_Location_2", settings.originalFileLocation2)) return false;
    if (!this->readableConfigFile->Set_Value("Original_File_Location_3", settings.originalFileLocation3)) return false;
    if (!this->readableConfigFile->Set_Value("Original_File_Slot", settings.originalFileSlot)) return false;
    if (!this->readableConfigFile->Set_Value("Compare_Size", settings.compareSize)) return false;
    if (!this->readableConfigFile->Set_Value("Never_Ask_For_Save_Location", settings.neverAskForSaveLocation)) return false;
    if (!this->readableConfigFile->Set_Value("Always_Ask_For_Save_Location", settings.alwaysAskForSaveLocation)) return false;
    if (!this->readableConfigFile->Set_Value("Verbose_Conflict_Output", settings.verboseConflictOutput)) return false;
    if (!this->readableConfigFile->Set_Value("Skip_Checksum_When_Creating_Patch", settings.skipChecksumWhenCreatingPatch)) return false;
    return this->readableConfigFile->Save_And_Close();
}

bool Settings_File::Load_Settings(Settings &settings) {
    this->Load_Default_Settings(settings);
    if (!this->readableConfigFile->Open(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_HEXAGON_SETTINGS_FILENAME)) return false;
    this->readableConfigFile->Get_Value("Default_Original_File_Open_Location", settings.defaultOriginalFileOpenLocation);
    this->readableConfigFile->Get_Value("Default_Patch_Open_Location", settings.defaultPatchOpenLocation);
    this->readableConfigFile->Get_Value("Default_File_Open_Location", settings.defaultFileOpenLocation);
    this->readableConfigFile->Get_Value("Default_Patch_Save_Location", settings.defaultPatchSaveLocation);
    this->readableConfigFile->Get_Value("Default_File_Save_Location", settings.defaultFileSaveLocation);
    this->readableConfigFile->Get_Value("Original_File_Location_1", settings.originalFileLocation1);
    this->readableConfigFile->Get_Value("Original_File_Location_2", settings.originalFileLocation2);
    this->readableConfigFile->Get_Value("Original_File_Location_3", settings.originalFileLocation3);
    this->readableConfigFile->Get_Value("Original_File_Slot", settings.originalFileSlot);
    this->readableConfigFile->Get_Value("Compare_Size", settings.compareSize);
    this->readableConfigFile->Get_Value("Never_Ask_For_Save_Location", settings.neverAskForSaveLocation);
    this->readableConfigFile->Get_Value("Always_Ask_For_Save_Location", settings.alwaysAskForSaveLocation);
    this->readableConfigFile->Get_Value("Verbose_Conflict_Output", settings.verboseConflictOutput);
    this->readableConfigFile->Get_Value("Skip_Checksum_When_Creating_Patch", settings.skipChecksumWhenCreatingPatch);
    if (!this->readableConfigFile->Discard_And_Close()) return false;

    //Make Sure Settings are Valid
    if (!QDir(settings.defaultOriginalFileOpenLocation).exists()) settings.defaultOriginalFileOpenLocation = this->applicationLocation;
    if (!QDir(settings.defaultPatchOpenLocation).exists()) settings.defaultPatchOpenLocation = this->applicationLocation;
    if (!QDir(settings.defaultFileOpenLocation).exists()) settings.defaultFileOpenLocation = this->applicationLocation;
    if (!QDir(settings.defaultPatchSaveLocation).exists()) settings.defaultPatchSaveLocation = this->applicationLocation;
    if (!QDir(settings.defaultFileSaveLocation).exists()) settings.defaultFileSaveLocation = this->applicationLocation;
    switch (settings.originalFileSlot) {
    default:
        return false;
    case 1:
        if (!QFileInfo(settings.originalFileLocation1).exists()) settings.originalFileLocation1 = QString();
        break;
    case 2:
        if (!QFileInfo(settings.originalFileLocation2).exists()) settings.originalFileLocation2 = QString();
        break;
    case 3:
        if (!QFileInfo(settings.originalFileLocation3).exists()) settings.originalFileLocation3 = QString();
        break;
    }
    return true;
}

void Settings_File::Load_Default_Settings(Settings &settings) {
    settings.defaultOriginalFileOpenLocation = this->applicationLocation;
    settings.defaultPatchOpenLocation = this->applicationLocation;
    settings.defaultFileOpenLocation = this->applicationLocation;
    settings.defaultPatchSaveLocation = this->applicationLocation;
    settings.defaultFileSaveLocation = this->applicationLocation;
    settings.originalFileLocation1 = QString();
    settings.originalFileLocation2 = QString();
    settings.originalFileLocation3 = QString();
    settings.originalFileSlot = 1;
    settings.compareSize = 5;
    settings.neverAskForSaveLocation = false;
    settings.alwaysAskForSaveLocation = false;
    settings.verboseConflictOutput = false;
    settings.skipChecksumWhenCreatingPatch = false;
}
