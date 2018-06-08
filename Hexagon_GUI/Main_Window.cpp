#include "Main_Window.h"
#include "ui_Main_Window.h"
#include "Common_Strings.h"
#include "Error_Messages.h"
#include "File_Dialog_Manager.h"
#include "Settings_File.h"
#include "String_Manipulator.h"
#include "Version.h"
#include <assert.h>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

Main_Window::Main_Window(QWidget *parent, Hexagon_Interface *hexagonPlugin, Error_Messages *errorMessages) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Main_Window)
{
    //Setup the GUI
    assert(hexagonPlugin);
    this->hexagonPlugin = hexagonPlugin;
    ui->setupUi(this);
    QString applicationLocation = QApplication::applicationDirPath();
    hexagonPlugin->Startup(this, applicationLocation);

    //Prepare Classes
    this->errorMessages = errorMessages;
    this->errorMessages->Update_Parent(this);
    this->settingsFile = new Settings_File(applicationLocation);
    this->settingsFile->Load_Default_Settings(this->settings);
    this->stringManipulator = new String_Manipulator();
    this->fileDialogManager = new File_Dialog_Manager(this, this->ui, this->errorMessages, &this->settings, this->stringManipulator);

    //Set the Window Title
    QString nameAndVersion = Common_Strings::STRING_HEXAGON+" "+Version::VERSION;
    this->setWindowTitle(nameAndVersion);

    //Set the About Text
    this->ui->textAbout->setHtml(nameAndVersion+Common_Strings::STRING_HTML_NEW_LINE+Common_Strings::STRING_ABOUT_DESCRIPTION+Common_Strings::STRING_HTML_NEW_LINE+Common_Strings::STRING_HTML_NEW_LINE+
                Common_Strings::STRING_CREATED_BY_COOLCORD+Common_Strings::STRING_HTML_NEW_LINE+Common_Strings::STRING_HTML_NEW_LINE+
                "<a href="+Common_Strings::STRING_UPDATES_LINK+">"+Common_Strings::STRING_UPDATES+"</a>"+Common_Strings::STRING_HTML_NEW_LINE+Common_Strings::STRING_HTML_NEW_LINE+
                "<a href="+Common_Strings::STRING_SOURCE_CODE_LINK+">"+Common_Strings::STRING_SOURCE_CODE+"</a>"+Common_Strings::STRING_HTML_NEW_LINE+Common_Strings::STRING_HTML_NEW_LINE);
    this->ui->textAbout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    //Load Settings
    this->Load_Settings();
}

Main_Window::~Main_Window() {
    delete this->ui;
    delete this->fileDialogManager;
    delete this->settingsFile;
    delete this->stringManipulator;
}

void Main_Window::on_btnApplyPatch_clicked() {
    //Open the Patch
    QString patchFileLocation = this->fileDialogManager->Get_Open_File_Location(File_Types::PATCH_FILE);
    if (patchFileLocation.isEmpty()) return;

    //Open the Original File to Patch
    QString originalFileLocation = this->ui->leOriginalFile->text();
    QFileInfo originalFileInfo(originalFileLocation);
    if (!originalFileInfo.isReadable()) {
        originalFileLocation = this->fileDialogManager->Get_Open_File_Location(Common_Strings::STRING_ORIGINAL);
        if (originalFileLocation.isEmpty()) return;
        originalFileInfo = QFileInfo(originalFileLocation);
    }

    //Choose Where to Save the New File
    QString outputFileLocation = this->stringManipulator->Get_Output_File_Path_From_Patch_And_Original_File_Paths(patchFileLocation, originalFileLocation);
    QFileInfo outputFileInfo(outputFileLocation);
    if (this->ui->cbAlwaysAskForSaveLocation->isChecked() || outputFileInfo.exists()) {
        outputFileLocation = this->fileDialogManager->Get_Save_File_Location(File_Types::ANY_FILE, this->stringManipulator->Get_Extension(originalFileInfo.fileName()));
        if (outputFileLocation.isEmpty()) return;
        outputFileInfo = QFileInfo(outputFileLocation);
    }

    //Run the Command via the Plugin
    int lineNum = 0;
    Hexagon_Error_Codes::Error_Code errorCode = this->hexagonPlugin->Apply_Hexagon_Patch(patchFileLocation, originalFileLocation, outputFileLocation, lineNum);
    switch (errorCode) {
    default: assert(false); return;
    case Hexagon_Error_Codes::OK: this->errorMessages->Show_Information(outputFileInfo.fileName()+" created!"); return;
    case Hexagon_Error_Codes::READ_ERROR: this->errorMessages->Show_Read_Error(originalFileInfo.fileName()); return;
    case Hexagon_Error_Codes::READ_MODIFIED_ERROR: assert(false); return; //this should never happen
    case Hexagon_Error_Codes::WRITE_ERROR: this->errorMessages->Show_Write_Error(outputFileInfo.fileName()); return;
    case Hexagon_Error_Codes::PARSE_ERROR: this->errorMessages->Show_Parse_Error(lineNum); return;
    case Hexagon_Error_Codes::CONFLICTS_DETECTED: assert(false); return; //this should never happen
    }
}

void Main_Window::on_btnCreatePatch_clicked() {
    //Open the Original File
    QString originalFileLocation = this->ui->leOriginalFile->text();
    QFileInfo originalFileInfo(originalFileLocation);
    if (!originalFileInfo.isReadable()) {
        originalFileLocation = this->fileDialogManager->Get_Open_File_Location(Common_Strings::STRING_ORIGINAL);
        if (originalFileLocation.isEmpty()) return;
        originalFileInfo = QFileInfo(originalFileLocation);
    }

    //Open the Modified File
    QString modifiedFileLocation = this->fileDialogManager->Get_Open_File_Location(Common_Strings::STRING_MODIFIED);
    if (modifiedFileLocation.isEmpty()) return;
    QFileInfo modifiedFileInfo = QFileInfo(modifiedFileLocation);

    //Choose Where to Save the New File
    QString outputFileLocation = modifiedFileInfo.path()+"/"+this->stringManipulator->Get_File_Name_Without_Extension(modifiedFileInfo.fileName())+Common_Strings::STRING_PATCH_EXTENSION;
    QFileInfo outputFileInfo(outputFileLocation);
    if (this->ui->cbAlwaysAskForSaveLocation->isChecked() || outputFileInfo.exists()) {
        outputFileLocation = this->fileDialogManager->Get_Save_File_Location(File_Types::ANY_FILE, this->stringManipulator->Get_Extension(originalFileInfo.fileName()));
        if (outputFileLocation.isEmpty()) return;
        outputFileInfo = QFileInfo(outputFileLocation);
    }

    //Run the Command via the Plugin
    int lineNum = 0;
    Hexagon_Error_Codes::Error_Code errorCode = this->hexagonPlugin->Create_Hexagon_Patch(originalFileLocation, modifiedFileLocation, outputFileLocation, lineNum);
    switch (errorCode) {
    default: assert(false); return;
    case Hexagon_Error_Codes::OK: this->errorMessages->Show_Information(outputFileInfo.fileName()+" created!"); return;
    case Hexagon_Error_Codes::READ_ERROR: this->errorMessages->Show_Read_Error(originalFileInfo.fileName()); return;
    case Hexagon_Error_Codes::READ_MODIFIED_ERROR: this->errorMessages->Show_Read_Error(modifiedFileInfo.fileName()); return;
    case Hexagon_Error_Codes::WRITE_ERROR: this->errorMessages->Show_Write_Error(outputFileInfo.fileName()); return;
    case Hexagon_Error_Codes::PARSE_ERROR: assert(false); return; //this should never happen
    case Hexagon_Error_Codes::CONFLICTS_DETECTED: assert(false); return; //this should never happen
    }
}

void Main_Window::on_btnCheckAgainstPatch_clicked() {
    //TODO: Write this...
}

void Main_Window::on_btnCheckAgainstFolder_clicked() {
    //TODO: Write this...
}

void Main_Window::on_btnConvertHEXPtoQtCode_clicked() {
    //TODO: Write this...
}

void Main_Window::on_btnConvertQtCodetoHEXP_clicked() {
    //TODO: Write this...
}

void Main_Window::on_tbOriginalFile_clicked() {
    QString openLocation = QFileInfo(this->ui->leOriginalFile->text()).absolutePath();
    if (!QFileInfo(openLocation).exists()) openLocation = this->settings.defaultFileOpenLocation;
    QString originalFileLocation = this->fileDialogManager->Get_Open_File_Location(Common_Strings::STRING_ORIGINAL);
    if (originalFileLocation.isEmpty()) return;
    else this->ui->leOriginalFile->setText(originalFileLocation);
}

void Main_Window::on_Main_Window_finished(int result) {
    if (result == 0) this->Save_Settings();
}

void Main_Window::Load_Settings() {
    this->settingsFile->Load_Settings(this->settings);
    this->ui->leOriginalFile->setText(this->settings.originalFileLocation);
    this->ui->sbCompareSize->setValue(this->settings.compareSize);
    this->ui->cbAlwaysAskForSaveLocation->setChecked(this->settings.alwaysAskForSaveLocation);
}

void Main_Window::Save_Settings() {
    this->settings.originalFileLocation = this->ui->leOriginalFile->text();
    this->settings.compareSize = this->ui->sbCompareSize->value();
    this->settings.alwaysAskForSaveLocation = this->ui->cbAlwaysAskForSaveLocation->isChecked();
    this->settingsFile->Save_Settings(this->settings);
}
