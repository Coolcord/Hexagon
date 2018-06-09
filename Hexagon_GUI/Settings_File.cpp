#include "Settings_File.h"
#include "Common_Strings.h"
#include <QDir>
#include <QFile>
#include <QTextStream>

Settings_File::Settings_File(const QString &applicationLocation) {
    this->applicationLocation = applicationLocation;
}

bool Settings_File::Save_Settings(const Settings &settings) {
    if (!QDir().mkpath(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG)) return false;
    QFile file(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_HEXAGON_SETTINGS_FILENAME);
    if (file.exists() && !file.remove()) return false;
    if (!file.open(QIODevice::ReadWrite)) return false;
    QTextStream stream(&file);
    stream << settings.defaultOriginalFileOpenLocation << Common_Strings::STRING_NEW_LINE;
    stream << settings.defaultPatchOpenLocation << Common_Strings::STRING_NEW_LINE;
    stream << settings.defaultFileOpenLocation << Common_Strings::STRING_NEW_LINE;
    stream << settings.defaultPatchSaveLocation << Common_Strings::STRING_NEW_LINE;
    stream << settings.defaultFileSaveLocation << Common_Strings::STRING_NEW_LINE;
    stream << settings.originalFileLocation << Common_Strings::STRING_NEW_LINE;
    stream << settings.compareSize << Common_Strings::STRING_NEW_LINE;
    stream << settings.alwaysAskForSaveLocation << Common_Strings::STRING_NEW_LINE;
    stream << settings.skipChecksumWhenCreatingPatch << Common_Strings::STRING_NEW_LINE;
    stream.flush();
    file.close();
    return true;
}

bool Settings_File::Load_Settings(Settings &settings) {
    QFile file(this->applicationLocation + "/" + Common_Strings::STRING_CONFIG + "/" + Common_Strings::STRING_HEXAGON_SETTINGS_FILENAME);
    if (!file.exists()) return true; //first time running the plugin
    if (!file.open(QIODevice::ReadWrite)) return false;
    bool valid = true;
    settings.defaultOriginalFileOpenLocation = file.readLine().trimmed();
    settings.defaultPatchOpenLocation = file.readLine().trimmed();
    settings.defaultFileOpenLocation = file.readLine().trimmed();
    settings.defaultPatchSaveLocation = file.readLine().trimmed();
    settings.defaultFileSaveLocation = file.readLine().trimmed();
    settings.originalFileLocation = file.readLine().trimmed();
    settings.compareSize = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    settings.alwaysAskForSaveLocation = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    settings.skipChecksumWhenCreatingPatch = file.readLine().trimmed().toInt(&valid); if (!valid) return false;

    //Make Sure Settings are Valid
    if (!QDir(settings.defaultOriginalFileOpenLocation).exists()) settings.defaultOriginalFileOpenLocation = this->applicationLocation;
    if (!QDir(settings.defaultPatchOpenLocation).exists()) settings.defaultPatchOpenLocation = this->applicationLocation;
    if (!QDir(settings.defaultFileOpenLocation).exists()) settings.defaultFileOpenLocation = this->applicationLocation;
    if (!QDir(settings.defaultPatchSaveLocation).exists()) settings.defaultPatchSaveLocation = this->applicationLocation;
    if (!QDir(settings.defaultFileSaveLocation).exists()) settings.defaultFileSaveLocation = this->applicationLocation;
    if (!QFileInfo(settings.originalFileLocation).exists()) settings.originalFileLocation = QString();
    return true;
}

void Settings_File::Load_Default_Settings(Settings &settings) {
    settings.defaultOriginalFileOpenLocation = this->applicationLocation;
    settings.defaultPatchOpenLocation = this->applicationLocation;
    settings.defaultFileOpenLocation = this->applicationLocation;
    settings.defaultPatchSaveLocation = this->applicationLocation;
    settings.defaultFileSaveLocation = this->applicationLocation;
    settings.originalFileLocation = QString();
    settings.compareSize = 5;
    settings.alwaysAskForSaveLocation = false;
    settings.skipChecksumWhenCreatingPatch = false;
}
