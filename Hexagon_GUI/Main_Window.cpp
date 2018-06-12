#include "Main_Window.h"
#include "ui_Main_Window.h"
#include "Common_Strings.h"
#include "Conflicts_Window.h"
#include "Error_Messages.h"
#include "File_Dialog_Manager.h"
#include "Settings_File.h"
#include "String_Manipulator.h"
#include "Version.h"
#include <assert.h>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include <QDebug>

Main_Window::Main_Window(QWidget *parent, Hexagon_Interface *hexagonPlugin, Error_Messages *errorMessages) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Main_Window)
{
    //Setup the GUI
    assert(hexagonPlugin);
    this->hexagonPlugin = hexagonPlugin;
    ui->setupUi(this);
    QString applicationLocation = QApplication::applicationDirPath();

    //Prepare Classes
    this->errorMessages = errorMessages;
    this->errorMessages->Update_Parent(this);
    this->settingsFile = new Settings_File(applicationLocation);
    this->settingsFile->Load_Default_Settings(this->settings);
    this->stringManipulator = new String_Manipulator();
    this->fileDialogManager = new File_Dialog_Manager(this, this->ui, applicationLocation, this->errorMessages, &this->settings, this->stringManipulator);

    //Set the Window Title
    this->setWindowTitle(Common_Strings::STRING_HEXAGON);

    //Set the About Text
    this->ui->textAbout->setHtml(Common_Strings::STRING_HEXAGON+" "+Version::VERSION+Common_Strings::STRING_HTML_NEW_LINE+Common_Strings::STRING_ABOUT_DESCRIPTION+Common_Strings::STRING_HTML_NEW_LINE+Common_Strings::STRING_HTML_NEW_LINE+
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
    QFileInfo patchFileInfo(patchFileLocation);

    //Open the Original File to Patch
    QString originalFileLocation = this->ui->leOriginalFile->text();
    QFileInfo originalFileInfo(originalFileLocation);
    if (!originalFileInfo.isReadable()) {
        originalFileLocation = this->fileDialogManager->Get_Open_File_Location(Common_Strings::STRING_ORIGINAL);
        if (originalFileLocation.isEmpty()) return;
        originalFileInfo = QFileInfo(originalFileLocation);
    }

    //Choose Where to Save the New File
    QString outputFileLocation = patchFileInfo.path()+this->stringManipulator->Get_File_Name_Without_Extension(patchFileInfo.fileName())+this->stringManipulator->Get_Extension(originalFileInfo.fileName());
    QFileInfo outputFileInfo(outputFileLocation);
    if (this->ui->cbAlwaysAskForSaveLocation->isChecked() || outputFileInfo.exists()) {
        outputFileLocation = this->fileDialogManager->Get_Save_File_Location(File_Types::ANY_FILE, this->stringManipulator->Get_Extension(originalFileInfo.fileName()));
        if (outputFileLocation.isEmpty()) return;
        outputFileInfo = QFileInfo(outputFileLocation);
    }

    //Run the Command via the Plugin
    int lineNum = 0;
    Hexagon_Error_Codes::Error_Code errorCode = this->hexagonPlugin->Apply_Hexagon_Patch(patchFileLocation, originalFileLocation, outputFileLocation, true, lineNum);
    if (errorCode == Hexagon_Error_Codes::BAD_CHECKSUM) {
        int result = QMessageBox::warning(this, Common_Strings::STRING_HEXAGON, Common_Strings::STRING_CHECKSUM_FAILED_APPLY_ANYWAY, QMessageBox::Yes, QMessageBox::No);
        if (result != QMessageBox::Yes) return;
        errorCode = this->hexagonPlugin->Apply_Hexagon_Patch(patchFileLocation, originalFileLocation, outputFileLocation, false, lineNum);
    }
    switch (errorCode) {
    default: assert(false); return;
    case Hexagon_Error_Codes::OK: this->errorMessages->Show_Information(outputFileInfo.fileName()+" created!"); return;
    case Hexagon_Error_Codes::READ_PATCH_ERROR: this->errorMessages->Show_Read_Error(patchFileInfo.fileName()); return;
    case Hexagon_Error_Codes::READ_ERROR: this->errorMessages->Show_Read_Error(originalFileInfo.fileName()); return;
    case Hexagon_Error_Codes::WRITE_ERROR: this->errorMessages->Show_Write_Error(outputFileInfo.fileName()); return;
    case Hexagon_Error_Codes::PARSE_ERROR: this->errorMessages->Show_Parse_Error(lineNum); return;
    case Hexagon_Error_Codes::OFFSET_OUT_OF_RANGE: this->errorMessages->Show_Error(Common_Strings::STRING_OUT_OF_RANGE+QString(lineNum)+"!");
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

    //Choose Where to Save the New Patch File
    QString outputFileLocation = modifiedFileInfo.path()+"/"+this->stringManipulator->Get_File_Name_Without_Extension(modifiedFileInfo.fileName())+Common_Strings::STRING_PATCH_EXTENSION;
    QFileInfo outputFileInfo(outputFileLocation);
    if (this->ui->cbAlwaysAskForSaveLocation->isChecked() || outputFileInfo.exists()) {
        outputFileLocation = this->fileDialogManager->Get_Save_File_Location(File_Types::PATCH_FILE);
        if (outputFileLocation.isEmpty()) return;
        outputFileInfo = QFileInfo(outputFileLocation);
    }

    //Run the Command via the Plugin
    Hexagon_Error_Codes::Error_Code errorCode = this->hexagonPlugin->Create_Hexagon_Patch(originalFileLocation, modifiedFileLocation, outputFileLocation,
                                                                                          this->ui->sbCompareSize->value(), !this->ui->cbSkipChecksumWhenCreatingPatch->isChecked(), !this->ui->cbSkipCommentsWhenCreatingPatch->isChecked());
    switch (errorCode) {
    default: assert(false); return;
    case Hexagon_Error_Codes::OK: this->errorMessages->Show_Information(outputFileInfo.fileName()+" created!"); return;
    case Hexagon_Error_Codes::READ_ERROR: this->errorMessages->Show_Read_Error(originalFileInfo.fileName()); return;
    case Hexagon_Error_Codes::READ_MODIFIED_ERROR: this->errorMessages->Show_Read_Error(modifiedFileInfo.fileName()); return;
    case Hexagon_Error_Codes::WRITE_ERROR: this->errorMessages->Show_Write_Error(outputFileInfo.fileName()); return;
    case Hexagon_Error_Codes::SIZE_DIFFERENCE: this->errorMessages->Show_Error(Common_Strings::STRING_FILES_ARE_DIFFERENT_SIZES);
    }
}

void Main_Window::on_btnCheckAgainstOtherPatches_clicked() {
    //Open the Patch
    QString patchFileLocation = this->fileDialogManager->Get_Open_File_Location(File_Types::PATCH_FILE);
    if (patchFileLocation.isEmpty()) return;

    //Open the Other Patches to Compare it Against
    QStringList otherPatchFileLocations = QFileDialog::getOpenFileNames(this, "Open Patch Files", this->settings.defaultPatchOpenLocation, Common_Strings::STRING_PATCH_EXTENSION_FILTER);
    return this->Check_For_Conflicts(patchFileLocation, otherPatchFileLocations);
}

void Main_Window::on_btnCheckAgainstFolder_clicked() {
    //Open the Patch
    QString patchFileLocation = this->fileDialogManager->Get_Open_File_Location(File_Types::PATCH_FILE);
    if (patchFileLocation.isEmpty()) return;

    //Open the Folder
    QString directoryLocation = QFileDialog::getExistingDirectory(this, "Open Folder", this->settings.defaultPatchOpenLocation);
    if (directoryLocation == NULL || directoryLocation.isEmpty() || !QDir(directoryLocation).isReadable()) return;

    //Get All Files in All Subdirectories
    QDirIterator it(directoryLocation, QStringList() << "*"+Common_Strings::STRING_PATCH_EXTENSION, QDir::Files, QDirIterator::Subdirectories);
    QStringList otherPatchFileLocations;
    while (it.hasNext()) otherPatchFileLocations.append(it.next());
    return this->Check_For_Conflicts(patchFileLocation, otherPatchFileLocations);
}

void Main_Window::on_btnConvertHEXPtoQtCode_clicked() {
    //Open the Patch File
    QString patchFileLocation = this->fileDialogManager->Get_Open_File_Location(File_Types::PATCH_FILE);
    if (patchFileLocation.isEmpty()) return;
    QFileInfo patchFileInfo = QFileInfo(patchFileLocation);

    //Choose Where to Save the New Patch File
    QString outputFileLocation = patchFileInfo.path()+"/"+this->stringManipulator->Get_File_Name_Without_Extension(patchFileInfo.fileName())+Common_Strings::STRING_TEXT_EXTENSION;
    QFileInfo outputFileInfo(outputFileLocation);
    if (this->ui->cbAlwaysAskForSaveLocation->isChecked() || outputFileInfo.exists()) {
        outputFileLocation = this->fileDialogManager->Get_Save_File_Location(File_Types::QT_CODE_FILE);
        if (outputFileLocation.isEmpty()) return;
        outputFileInfo = QFileInfo(outputFileLocation);
    }

    //Run the Command via the Plugin
    int lineNum = 0;
    Hexagon_Error_Codes::Error_Code errorCode = this->hexagonPlugin->Convert_Hexagon_Patch_To_Qt_Code(patchFileLocation, outputFileLocation, lineNum);
    switch (errorCode) {
    default: assert(false); return;
    case Hexagon_Error_Codes::OK: this->errorMessages->Show_Information(outputFileInfo.fileName()+" created!"); return;
    case Hexagon_Error_Codes::READ_ERROR: this->errorMessages->Show_Read_Error(patchFileInfo.fileName()); return;
    case Hexagon_Error_Codes::WRITE_ERROR: this->errorMessages->Show_Write_Error(outputFileInfo.fileName()); return;
    case Hexagon_Error_Codes::PARSE_ERROR: this->errorMessages->Show_Parse_Error(lineNum); return;
    }
}

void Main_Window::on_btnConvertQtCodetoHEXP_clicked() {
    //Open the Qt Code File
    QString qtCodeFileLocation = this->fileDialogManager->Get_Open_File_Location(File_Types::QT_CODE_FILE);
    if (qtCodeFileLocation.isEmpty()) return;
    QFileInfo qtCodeFileInfo = QFileInfo(qtCodeFileLocation);

    //Choose Where to Save the New Patch File
    QString outputFileLocation = qtCodeFileInfo.path()+"/"+this->stringManipulator->Get_File_Name_Without_Extension(qtCodeFileInfo.fileName())+Common_Strings::STRING_PATCH_EXTENSION;
    QFileInfo outputFileInfo(outputFileLocation);
    if (this->ui->cbAlwaysAskForSaveLocation->isChecked() || outputFileInfo.exists()) {
        outputFileLocation = this->fileDialogManager->Get_Save_File_Location(File_Types::PATCH_FILE);
        if (outputFileLocation.isEmpty()) return;
        outputFileInfo = QFileInfo(outputFileLocation);
    }

    //Run the Command via the Plugin
    int lineNum = 0;
    Hexagon_Error_Codes::Error_Code errorCode = this->hexagonPlugin->Convert_Qt_Code_To_Hexagon_Patch(qtCodeFileLocation, outputFileLocation, lineNum, !this->ui->cbSkipCommentsWhenCreatingPatch->isChecked());
    switch (errorCode) {
    default: assert(false); return;
    case Hexagon_Error_Codes::OK: this->errorMessages->Show_Information(outputFileInfo.fileName()+" created!"); return;
    case Hexagon_Error_Codes::READ_ERROR: this->errorMessages->Show_Read_Error(qtCodeFileInfo.fileName()); return;
    case Hexagon_Error_Codes::WRITE_ERROR: this->errorMessages->Show_Write_Error(outputFileInfo.fileName()); return;
    case Hexagon_Error_Codes::PARSE_ERROR: this->errorMessages->Show_Parse_Error(lineNum); return;
    }
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

void Main_Window::Check_For_Conflicts(const QString &patchFileLocation, const QStringList &otherPatchFileLocations) {
    for (int i = 0; i < otherPatchFileLocations.size(); ++i) qDebug() << otherPatchFileLocations.at(i);

    //Run the Command via the Plugin
    if (otherPatchFileLocations.isEmpty()) return;
    int lineNum = 0, otherLineNum = 0, otherFileNum = 0;
    QString output = QString();
    Hexagon_Error_Codes::Error_Code errorCode = this->hexagonPlugin->Check_For_Conflicts_Between_Hexagon_Patches(patchFileLocation, otherPatchFileLocations, output, lineNum, otherLineNum, otherFileNum, this->ui->cbVerboseConflictOutput->isChecked());
    switch (errorCode) {
    default: assert(false); return;
    case Hexagon_Error_Codes::OK: this->errorMessages->Show_Information("No conflicts detected!"); return;
    case Hexagon_Error_Codes::READ_ERROR: this->errorMessages->Show_Parse_Error(QFileInfo(patchFileLocation).fileName(), lineNum); return;
    case Hexagon_Error_Codes::READ_MODIFIED_ERROR: this->errorMessages->Show_Parse_Error(QFileInfo(otherPatchFileLocations.at(otherFileNum)).fileName(), otherLineNum); return;
    case Hexagon_Error_Codes::CONFLICTS_DETECTED:
        Conflicts_Window(this, output).exec();
        return;
    }
}

void Main_Window::Load_Settings() {
    this->settingsFile->Load_Settings(this->settings);
    this->ui->leOriginalFile->setText(this->settings.originalFileLocation);
    this->ui->sbCompareSize->setValue(this->settings.compareSize);
    this->ui->cbAlwaysAskForSaveLocation->setChecked(this->settings.alwaysAskForSaveLocation);
    this->ui->cbVerboseConflictOutput->setChecked(this->settings.verboseConflictOutput);
    this->ui->cbSkipChecksumWhenCreatingPatch->setChecked(this->settings.skipChecksumWhenCreatingPatch);
    this->ui->cbSkipCommentsWhenCreatingPatch->setChecked(this->settings.skipCommentsWhenCreatingPatch);
}

void Main_Window::Save_Settings() {
    this->settings.originalFileLocation = this->ui->leOriginalFile->text();
    this->settings.compareSize = this->ui->sbCompareSize->value();
    this->settings.alwaysAskForSaveLocation = this->ui->cbAlwaysAskForSaveLocation->isChecked();
    this->settings.verboseConflictOutput = this->ui->cbVerboseConflictOutput->isChecked();
    this->settings.skipChecksumWhenCreatingPatch = this->ui->cbSkipChecksumWhenCreatingPatch->isChecked();
    this->settings.skipCommentsWhenCreatingPatch = this->ui->cbSkipCommentsWhenCreatingPatch->isChecked();
    this->settingsFile->Save_Settings(this->settings);
}
