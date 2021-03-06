#ifndef PATCH_STRINGS_H
#define PATCH_STRINGS_H

#include <QString>

namespace Patch_Strings {
    const static QString STRING_CHECKSUM = "C:";
    const static QString STRING_SIZE = "S:";
    const static QString STRING_COMMENT = "#";
    const static QString STRING_HEX_IDENTIFIER = "0x";
    const static QString STRING_OFFSET = "A:";
    const static QString STRING_SKIP_CHECKSUM = "SKIP";
    const static QString STRING_VALUE = "V:";
    const static QString STRING_VALUE_TAB = "   ";

    //Header
    const static QString STRING_HEADER = "Hexagon Patch";
    const static QString STRING_CREATED = "Created on";
    const static QString STRING_ORIGINAL_FILE_NAME = "Original File Name: ";

    //Conflicts
    const static QString STRING_THE_FOLLOWING_PATCHES_ARE_COMPATIBLE_WITH = "The following patches are compatible with ";
    const static QString STRING_THE_FOLLOWING_PATCHES_ARE_INCOMPATIBLE_WITH = "The following patches are incompatible with ";

    #ifdef Q_OS_WIN32
    const static QString STRING_NEW_LINE = "\r\n";
    #else
    const static QString STRING_NEW_LINE = "\n";
    #endif
}

#endif // PATCH_STRINGS_H
