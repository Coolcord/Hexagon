#ifndef PATCH_STRINGS_H
#define PATCH_STRINGS_H

#include <QString>

namespace Patch_Strings {
    const static QString STRING_CHECKSUM = "C:";
    const static QString STRING_COMMENT = "#";
    const static QString STRING_HEX_IDENTIFIER = "0x";
    const static QString STRING_OFFSET = "A:";
    const static QString STRING_SKIP_CHECKSUM = "SKIP";
    const static QString STRING_VALUE = "V:";
    const static QString STRING_VALUE_TAB = "   ";
    const static QString STRING_BREAK = "=";

    //Header
    const static QString STRING_HEADER = "Hexagon Patch";
    const static QString STRING_CREATED = "Created on";

    #ifdef Q_OS_WIN32
    const static QString STRING_NEW_LINE = "\r\n";
    #else
    const static QString STRING_NEW_LINE = "\n";
    #endif
}

#endif // PATCH_STRINGS_H
