#include "Value_Manipulator.h"
#include "Patch_Strings.h"

QString Value_Manipulator::Convert_QByteArray_To_QString(const QByteArray &bytes) {
    QString output = QString();
    for (int i = 0; i < bytes.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(bytes.at(i));
        unsigned char firstNibble = c&0xF0;
        unsigned char secondNibble = c&0x0F;
        output += QString(this->Get_Char_From_Nibble(firstNibble))+QString(this->Get_Char_From_Nibble(secondNibble));
    }
    return output;
}

QString Value_Manipulator::Convert_Offset_To_QString_With_Padding(qint64 offset, int numberOfDigits) {
    QString offsetString = QString::number(offset);
    if (offsetString.size() >= numberOfDigits) return offsetString;
    QString padding = QString(QByteArray(numberOfDigits-offsetString.size(), '0'));
    offsetString = padding+offsetString;
    return offsetString;
}

bool Value_Manipulator::Convert_QString_To_QByteArray(const QString &string, QByteArray &output) {
    if (string.size()%2 == 1) return false;
    output = QByteArray(string.size()/2, 0x00);
    for (int i = 0, j = 0; i < string.size(); i += 2, ++j) {
        assert(j < output.size());
        QString numberString = QString(string.at(i))+QString(string.at(i+1));
        bool valid = false;
        int number = numberString.toInt(&valid, 0x10);
        if (!valid) return false;
        assert(numberString >= 0x00 && numberString <= 0xFF);
        output.data()[j] = static_cast<char>(number);
    }
    return true;
}

bool Value_Manipulator::Is_Line_Hex_String(const QString &line) {
    for (int i = 0; i < line.size(); ++i) {
        switch (line.at(i).toLower().toLatin1()) {
        default: return false;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
        case '8': case '9': case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': continue;
        }
    }
    return true;
}

QString Value_Manipulator::Trim_Hex_Identifier(QString &hexString) {
    if (hexString.startsWith(Patch_Strings::STRING_HEX_IDENTIFIER)) {
        int hexIdentifierSize = Patch_Strings::STRING_HEX_IDENTIFIER.size();
        return hexString.remove(hexIdentifierSize, hexString.size()-hexIdentifierSize);
    }
    return hexString;
}

char Value_Manipulator::Get_Char_From_Nibble(unsigned char nibble) {
    assert(nibble < 0x10);
    switch(nibble) {
    default: assert(false);
    case 0x0: return '0';
    case 0x1: return '1';
    case 0x2: return '2';
    case 0x3: return '3';
    case 0x4: return '4';
    case 0x5: return '5';
    case 0x6: return '6';
    case 0x7: return '7';
    case 0x8: return '8';
    case 0x9: return '9';
    case 0xA: return 'A';
    case 0xB: return 'B';
    case 0xC: return 'C';
    case 0xD: return 'D';
    case 0xE: return 'E';
    case 0xF: return 'F';
    }
}
