#include "Main_Window.h"
#include "ui_Main_Window.h"
#include "Common_Strings.h"
#include "String_Manipulator.h"
#include "Version.h"
#include <assert.h>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

Main_Window::Main_Window(QWidget *parent, Hexagon_Interface *hexagonPlugin) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Main_Window)
{
    //Setup the GUI
    assert(hexagonPlugin);
    this->hexagonPlugin = hexagonPlugin;
    ui->setupUi(this);
    hexagonPlugin->Startup(this, QApplication::applicationDirPath());
    this->defaultFileOpenLocation = QApplication::applicationDirPath();
    this->defaultPatchOpenLocation = this->defaultFileOpenLocation;

    //Prepare Classes
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
}

Main_Window::~Main_Window() {
    delete this->ui;
    delete this->stringManipulator;
}

void Main_Window::on_btnApplyPatch_clicked() {
    //Open the Patch
    QString patchFileLocation = QFileDialog::getOpenFileName(this, "Open a "+Common_Strings::STRING_HEXAGON+" Patch", this->defaultPatchOpenLocation,
                                                        Common_Strings::STRING_PATCH_EXTENSION_FILTER);
    if (patchFileLocation == NULL || patchFileLocation.isEmpty()) return;
    QFileInfo patchFileInfo(patchFileLocation);
    if (!patchFileInfo.isReadable()) {
        //TODO: Show Read/Write Error...
        return;
    }
    this->defaultPatchOpenLocation = patchFileInfo.path();

    //Open the Original File to Patch
    QString originalFileLocation = this->ui->leOriginalFile->text();
    QFileInfo originalFileInfo(originalFileLocation);
    if (!originalFileInfo.isReadable()) {
        originalFileLocation = QFileDialog::getOpenFileName(this, "Open File", this->defaultFileOpenLocation,
                                                            Common_Strings::STRING_ALL_FILE_EXTENSION_FILTER);
        if (originalFileLocation == NULL || originalFileLocation.isEmpty()) return;
        originalFileInfo = QFileInfo(originalFileLocation);
        if (!originalFileInfo.isReadable()) {
            //TODO: Show Read/Write Error...
            return;
        }
    }
    this->defaultFileOpenLocation = originalFileInfo.path();
    QString originalFileExtension = originalFileInfo.fileName().split('.').last();

    //Choose Where to Save the New File
    QStringList names = patchFileInfo.fileName().split('.');
    QString patchFileNameWithoutExtension = QString();
    for (unsigned int i = 0; i < names.size()-1; ++i) {
        patchFileNameWithoutExtension += names.at(i);
        if (i != names.size()-2) patchFileNameWithoutExtension+".";
    }
    QString outputFileLocation = originalFileInfo.path()+"/"+patchFileNameWithoutExtension+"."+originalFileExtension;
    //QFileInfo outputFileInfo;
    if (this->ui->cbAskForSaveLocation->isChecked()) {
        outputFileLocation = QFileDialog::getSaveFileName(this, "Save File", this->defaultPatchOpenLocation,
                                                            "(*."+originalFileExtension+")");
        if (outputFileLocation == NULL || outputFileLocation.isEmpty()) return;
        outputFileInfo = QFileInfo(outputFileLocation);
    } else { //save the file in the same folder as the patch
        //TODO: Write this...
    }

    //QMessageBox::information(this, Common_Strings::STRING_HEXAGON, "Apply Patch clicked!", Common_Strings::STRING_OK);

    //TODO: Write this...
    int lineNum = 0;
    Hexagon_Error_Codes::Error_Code errorCode = this->hexagonPlugin->Apply_Hexagon_Patch(patchFileLocation, originalFileLocation, outputFileLocation, lineNum);
}

void Main_Window::on_btnCreatePatch_clicked() {
    QMessageBox::information(this, Common_Strings::STRING_HEXAGON, "Create Patch clicked!", Common_Strings::STRING_OK);

    //TODO: Write this...
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
    if (!QFileInfo(openLocation).exists()) openLocation = this->defaultFileOpenLocation;
    QString originalFileLocation = QFileDialog::getOpenFileName(this, "Open File", openLocation, "All files (*.*)");
    if (originalFileLocation.isEmpty()) return;
    else this->ui->leOriginalFile->setText(originalFileLocation);
}

void Main_Window::on_cbAskForSaveLocation_clicked(bool checked) {
    //TODO: Write this...
}

void Main_Window::on_Main_Window_finished(int result) {
    //TODO: Write this...
}
