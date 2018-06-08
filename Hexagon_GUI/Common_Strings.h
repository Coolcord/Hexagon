#ifndef COMMON_STRINGS_H
#define COMMON_STRINGS_H

#include <QString>

namespace Common_Strings {
    #ifdef Q_OS_WIN32
    const static QString STRING_PLUGIN_EXTENSION = ".dll"; //Windows uses .dll files
    const static QString STRING_NEW_LINE = "\r\n"; //Qt's endl seems to use only \n for some reason... I'll look into this later
    #else
    const static QString STRING_PLUGIN_EXTENSION = ".so"; //Unix uses .so files
    const static QString STRING_NEW_LINE = "\n";
    #endif

    const static QString STRING_HEXAGON = "Hexagon";
    const static QString STRING_PATCH_EXTENSION = ".hexp";
    const static QString STRING_PATCH_EXTENSION_FILTER = STRING_HEXAGON+" Patches (*"+STRING_PATCH_EXTENSION+")";
    const static QString STRING_TEXT_EXTENSION = ".txt";
    const static QString STRING_TEXT_EXTENSION_FILTER = "Text files (*.txt)";
    const static QString STRING_ALL_FILE_EXTENSION_FILTER = "All files (*.*)";
    const static QString STRING_PLUGIN_LOCATION = "./Plugins/"+STRING_HEXAGON+STRING_PLUGIN_EXTENSION;
    const static QString STRING_CONFIG = "Config";
    const static QString STRING_HEXAGON_SETTINGS_FILENAME = STRING_HEXAGON+".cfg";
    const static QString STRING_CONFIG_LOCATION = "./"+STRING_CONFIG+"/"+STRING_HEXAGON_SETTINGS_FILENAME;
    const static QString STRING_OK = "OK";
    const static QString STRING_ORIGINAL = "ORIGINAL";
    const static QString STRING_MODIFIED = "MODIFIED";

    //About Strings
    const static QString STRING_ABOUT_DESCRIPTION = "A Simple Plain Text Hexadecimal Patcher";
    const static QString STRING_CREATED_BY_COOLCORD = "Created by Coolcord";
    const static QString STRING_GET_UPDATES = "Get Updates";
    const static QString STRING_UPDATES = "Updates";
    const static QString STRING_UPDATES_LINK = "https://github.com/Coolcord/Hexagon/releases";
    const static QString STRING_SOURCE_CODE = "Source Code";
    const static QString STRING_SOURCE_CODE_LINK = "https://github.com/Coolcord/Hexagon";
    const static QString STRING_HTML_NEW_LINE = "<br/>";
}

#endif // COMMON_STRINGS_H
