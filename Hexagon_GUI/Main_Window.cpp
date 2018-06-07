#include "Main_Window.h"
#include "ui_Main_Window.h"
#include "Common_Strings.h"
#include "Error_Messages.h"
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
    hexagonPlugin->Startup(this, QApplication::applicationDirPath());
    this->settings.defaultFileOpenLocation = QApplication::applicationDirPath();
    this->settings.defaultPatchOpenLocation = this->settings.defaultFileOpenLocation;

    //Prepare Classes
    this->errorMessages = errorMessages;
    this->errorMessages->Update_Parent(this);
    this->settingsFile = new Settings_File();
    this->stringManipulator = new String_Manipulator();

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
    this->settingsFile->Load_Settings(this->settings);
}

Main_Window::~Main_Window() {
    delete this->ui;
    delete this->settingsFile;
    delete this->stringManipulator;
}

void Main_Window::on_btnApplyPatch_clicked() {
    //Open the Patch
    QString patchFileLocation = QFileDialog::getOpenFileName(this, "Open a "+Common_Strings::STRING_HEXAGON+" Patch", this->settings.defaultPatchOpenLocation,
                                                        Common_Strings::STRING_PATCH_EXTENSION_FILTER);
    if (patchFileLocation == NULL || patchFileLocation.isEmpty()) return;
    QFileInfo patchFileInfo(patchFileLocation);
    if (!patchFileInfo.isReadable()) {
        this->errorMessages->Show_Read_Error(patchFileInfo.fileName());
        return;
    }
    this->settings.defaultPatchOpenLocation = patchFileInfo.path();

    //Open the Original File to Patch
    QString originalFileLocation = this->ui->leOriginalFile->text();
    QFileInfo originalFileInfo(originalFileLocation);
    if (!originalFileInfo.isReadable()) {
        originalFileLocation = QFileDialog::getOpenFileName(this, "Open File", this->settings.defaultFileOpenLocation,
                                                            Common_Strings::STRING_ALL_FILE_EXTENSION_FILTER);
        if (originalFileLocation == NULL || originalFileLocation.isEmpty()) return;
        originalFileInfo = QFileInfo(originalFileLocation);
        if (!originalFileInfo.isReadable()) {
            this->errorMessages->Show_Read_Error(originalFileInfo.fileName());
            return;
        }
    }
    this->settings.defaultFileOpenLocation = originalFileInfo.path();
    QString originalFileExtension = this->stringManipulator->Get_Extension(originalFileInfo.fileName());

    //Choose Where to Save the New File
    QString outputFileLocation = this->stringManipulator->Get_Output_File_Path_From_Patch_And_Original_Paths(patchFileLocation, originalFileLocation);
    QFileInfo outputFileInfo(outputFileLocation);
    if (this->ui->cbAlwaysAskForSaveLocation->isChecked() || outputFileInfo.exists()) {
        outputFileLocation = QFileDialog::getSaveFileName(this, "Save File", this->settings.defaultPatchOpenLocation,
                                                            "(*."+originalFileExtension+")");
        if (outputFileLocation == NULL || outputFileLocation.isEmpty()) return;
        outputFileInfo = QFileInfo(outputFileLocation);
    }
    if (!outputFileInfo.isWritable()) {
        this->errorMessages->Show_Write_Error(outputFileInfo.fileName());
        return;
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
    QMessageBox::information(this, Common_Strings::STRING_HEXAGON, "Create Patch clicked!", Common_Strings::STRING_OK);

    //TODO: Write this...
    //TODO: Abstract Apply so that most of the code can be reused...

    QString originalFileLocation;
    QFileInfo originalFileInfo;
    QString modifiedFileLocation;
    QFileInfo modifiedFileInfo;
    QString outputFileLocation;
    QFileInfo outputFileInfo;

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
    QString originalFileLocation = QFileDialog::getOpenFileName(this, "Open File", openLocation, "All files (*.*)");
    if (originalFileLocation.isEmpty()) return;
    else this->ui->leOriginalFile->setText(originalFileLocation);
}

void Main_Window::on_Main_Window_finished(int result) {
    this->settingsFile->Save_Settings(this->settings);
}
