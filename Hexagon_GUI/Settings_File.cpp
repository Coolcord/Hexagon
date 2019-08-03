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
    stream << settings.originalFileLocation1 << Common_Strings::STRING_NEW_LINE;
    stream << settings.originalFileLocation2 << Common_Strings::STRING_NEW_LINE;
    stream << settings.originalFileLocation3 << Common_Strings::STRING_NEW_LINE;
    stream << settings.originalFileSlot << Common_Strings::STRING_NEW_LINE;
    stream << settings.compareSize << Common_Strings::STRING_NEW_LINE;
    stream << settings.neverAskForSaveLocation << Common_Strings::STRING_NEW_LINE;
    stream << settings.alwaysAskForSaveLocation << Common_Strings::STRING_NEW_LINE;
    stream << settings.verboseConflictOutput << Common_Strings::STRING_NEW_LINE;
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
    settings.originalFileLocation1 = file.readLine().trimmed();
    settings.originalFileLocation2 = file.readLine().trimmed();
    settings.originalFileLocation3 = file.readLine().trimmed();
    settings.originalFileSlot = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    settings.compareSize = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    settings.neverAskForSaveLocation = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    settings.alwaysAskForSaveLocation = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    settings.verboseConflictOutput = file.readLine().trimmed().toInt(&valid); if (!valid) return false;
    settings.skipChecksumWhenCreatingPatch = file.readLine().trimmed().toInt(&valid); if (!valid) return false;

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
