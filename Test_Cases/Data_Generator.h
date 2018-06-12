#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include <QString>

class Data_Generator {
public:
    Data_Generator(const QString &applicationLocation);
    ~Data_Generator() {}
    bool Generate_Data();
    bool Clean_Up();
    bool Generate_Patch_1();
    bool Generate_Patch_2();
    bool Generate_Binary_File();

private:
    QString applicationLocation;
};

#endif // DATA_GENERATOR_H
