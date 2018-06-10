#ifndef QT_CODE_STRINGS_H
#define QT_CODE_STRINGS_H

#include <QString>

namespace Qt_Code_Strings {
    //Needed for Everything
    const static QString STRING_IF_NOT = "if (!";
    const static QString STRING_WRITE_BYTES_TO_OFFSET = "Write_Bytes_To_Offset(";
    const static QString STRING_RETURN_FALSE = " return false;";
    const static QString STRING_SEMI_COLON = ";";
    const static QString STRING_RETURN = "return ";

    //Standard Qt From Hex
    const static QString STRING_FROM_HEX = ", QByteArray::fromHex(QString(";
    const static QString STRING_TO_LATIN = ").toLatin1())))";

    //Using a Byte Array
    const static QString STRING_BYTE_ARRAY = ", QByteArray(";
    const static QString STRING_COMMA = ", ";
    const static QString STRING_ENDING_PARENTHESES = ")))";
}

#endif // QT_CODE_STRINGS_H
