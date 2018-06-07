#ifndef STRING_MANIPULATOR_H
#define STRING_MANIPULATOR_H

#include <QString>

class String_Manipulator {
public:
    String_Manipulator() {}
    QString Get_Extension(const QString &fileName);
    QString Get_File_Name_Without_Extension(const QString &fileName);
    QString Get_File_Name_Without_Path(const QString &filePath);
    QString Get_Output_File_Path_From_Patch_And_Original_Paths(const QString &patchFileName, const QString &originalFilePath);
    QString Get_Path_Without_File_Name(const QString &path);

private:
    QString Get_Everything_Before_Last_Character(const QString &string, char c);
};

#endif // STRING_MANIPULATOR_H
