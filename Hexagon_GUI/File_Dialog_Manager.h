#ifndef FILE_DIALOG_MANAGER_H
#define FILE_DIALOG_MANAGER_H

#include "File_Types.h"
#include "Settings.h"
#include <QWidget>

class Error_Messages;
class String_Manipulator;
namespace Ui { class Main_Window; }

class File_Dialog_Manager
{
public:
    File_Dialog_Manager(QWidget *parent, Ui::Main_Window *ui, Error_Messages *errorMessages, Settings *settings, String_Manipulator *stringManipulator);
    ~File_Dialog_Manager() {}
    QString Get_Open_File_Location(File_Types::File_Type fileType);
    QString Get_Open_File_Location(File_Types::File_Type fileType, QString extension);
    QString Get_Open_File_Location(QString message);
    QString Get_Open_File_Location(QString message, QString extension);
    QString Get_Save_File_Location(File_Types::File_Type fileType);
    QString Get_Save_File_Location(File_Types::File_Type fileType, QString extension);
    QString Get_Save_File_Location(QString message);
    QString Get_Save_File_Location(QString message, QString extension);

private:
    QString Get_File_Location(File_Types::File_Type fileType, QString message, QString extension, bool save);

    QWidget *parent;
    Error_Messages *errorMessages;
    Ui::Main_Window *ui;
    Settings *settings;
    String_Manipulator *stringManipulator;
};

#endif // FILE_DIALOG_MANAGER_H
