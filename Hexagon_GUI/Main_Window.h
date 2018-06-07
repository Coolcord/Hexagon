#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../Hexagon/Hexagon_Interface.h"
#include "Settings.h"
#include <QDialog>
#include <QString>

class Error_Messages;
class File_Dialog_Manager;
class Settings_File;
class String_Manipulator;

namespace Ui {
class Main_Window;
}

class Main_Window : public QDialog {
    Q_OBJECT

public:
    Main_Window(QWidget *parent, Hexagon_Interface *hexagonPlugin, Error_Messages *errorMessages);
    ~Main_Window();

private slots:
    void on_btnApplyPatch_clicked();
    void on_btnCreatePatch_clicked();
    void on_btnCheckAgainstPatch_clicked();
    void on_btnCheckAgainstFolder_clicked();
    void on_btnConvertHEXPtoQtCode_clicked();
    void on_btnConvertQtCodetoHEXP_clicked();
    void on_tbOriginalFile_clicked();
    void on_Main_Window_finished(int result);

private:
    Ui::Main_Window *ui;
    Hexagon_Interface *hexagonPlugin;
    Error_Messages *errorMessages;
    File_Dialog_Manager *fileDialogManager;
    Settings settings;
    Settings_File *settingsFile;
    String_Manipulator *stringManipulator;
    QString defaultFileOpenLocation;
    QString defaultPatchOpenLocation;
};

#endif // MAIN_WINDOW_H
