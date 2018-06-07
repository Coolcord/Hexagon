#include "Main_Window.h"
#include "ui_Main_Window.h"
#include "Common_Strings.h"
#include "Version.h"
#include <QFileDialog>
#include <QMessageBox>

Main_Window::Main_Window(QWidget *parent) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Main_Window)
{
    ui->setupUi(this);
    this->defaultFileOpenLocation = QApplication::applicationDirPath();
    this->defaultPatchOpenLocation = this->defaultFileOpenLocation;

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
}

void Main_Window::on_btnApplyPatch_clicked() {
    QString patchFileLocation = QFileDialog::getOpenFileName(this, "Open a "+Common_Strings::STRING_HEXAGON+" Patch", this->defaultPatchOpenLocation,
                                                        Common_Strings::STRING_HEXAGON+" Patches (*."+Common_Strings::STRING_PATCH_EXTENSION+")");
    if (patchFileLocation == NULL || patchFileLocation.isEmpty()) return;
    //QString originalFileLocation;
    //QString outputFileLocation;

    //QMessageBox::information(this, Common_Strings::STRING_HEXAGON, "Apply Patch clicked!", Common_Strings::STRING_OK);

    //TODO: Write this...
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

void Main_Window::on_leOriginalFile_textChanged(const QString &arg1) {
    //TODO: Write this...
}

void Main_Window::on_tbOriginalFile_clicked() {
    //TODO: Write this...
}

void Main_Window::on_sbCompareSize_valueChanged(int arg1) {
    //TODO: Write this...
}

void Main_Window::on_cbAskForSaveLocation_clicked(bool checked) {
    //TODO: Write this...
}
