#include "String_Manipulator.h"
#include <assert.h>
#include <QStringList>

QString String_Manipulator::Get_Extension(const QString &fileName) {
    if (fileName.isEmpty()) return QString();
    return fileName.split('.').last();
}

QString String_Manipulator::Get_File_Name_Without_Extension(const QString &fileName) {
    return this->Get_Everything_Before_Last_Character(fileName, '.');
}

QString String_Manipulator::Get_File_Name_Without_Path(const QString &filePath) {
    if (filePath.isEmpty()) return QString();
    return filePath.split('/').last();
}

QString String_Manipulator::Get_Path_Without_File_Name(const QString &path) {
    return this->Get_Everything_Before_Last_Character(path, '/');
}

QString String_Manipulator::Get_Everything_Before_Last_Character(const QString &string, char c) {
    if (string.isEmpty()) return QString();
    QStringList strings = string.split(c);
    QString newString = QString();
    for (int i = 0; i < strings.size()-1; ++i) {
        newString += strings.at(i);
        if (i != strings.size()-2) newString += QString(c);
    }
    return newString;
}
