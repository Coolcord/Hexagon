#include "File_Dialog_Manager.h"
#include "Common_Strings.h"
#include "Error_Messages.h"
#include "String_Manipulator.h"
#include <assert.h>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

File_Dialog_Manager::File_Dialog_Manager(QWidget *parent, Ui::Main_Window *ui, const QString &applicationLocation,
                                         Error_Messages *errorMessages, Settings *settings, String_Manipulator *stringManipulator) {
    assert(parent); assert(ui); assert(errorMessages); assert(settings); assert(stringManipulator);
    this->parent = parent;
    this->ui = ui;
    this->applicationLocation = applicationLocation;
    this->errorMessages = errorMessages;
    this->settings = settings;
    this->stringManipulator = stringManipulator;
}

QString File_Dialog_Manager::Get_Open_File_Location(File_Types::File_Type fileType) {
    return this->Get_File_Location(fileType, QString(), QString(), false);
}

QString File_Dialog_Manager::Get_Open_File_Location(File_Types::File_Type fileType, QString extension) {
    return this->Get_File_Location(fileType, QString(), extension, false);
}

QString File_Dialog_Manager::Get_Open_File_Location(QString message) {
    return this->Get_File_Location(File_Types::ANY_FILE, message, QString(), false);
}

QString File_Dialog_Manager::Get_Open_File_Location(QString message, QString extension) {
    return this->Get_File_Location(File_Types::ANY_FILE, message, extension, false);
}

QString File_Dialog_Manager::Get_Save_File_Location(File_Types::File_Type fileType) {
    return this->Get_File_Location(fileType, QString(), QString(), true);
}

QString File_Dialog_Manager::Get_Save_File_Location(File_Types::File_Type fileType, QString extension) {
    return this->Get_File_Location(fileType, QString(), extension, true);
}

QString File_Dialog_Manager::Get_Save_File_Location(QString message) {
    return this->Get_File_Location(File_Types::ANY_FILE, message, QString(), true);
}

QString File_Dialog_Manager::Get_Save_File_Location(QString message, QString extension) {
    return this->Get_File_Location(File_Types::ANY_FILE, message, extension, true);
}

QString File_Dialog_Manager::Get_File_Location(File_Types::File_Type fileType, QString message, QString extension, bool save) {
    QString windowTitle = "Open ";
    if (save) windowTitle = "Save ";
    if (!message.isEmpty()) windowTitle += message+" ";
    QString extensionFilter = QString();
    QString openLocation = this->settings->defaultFileOpenLocation;
    switch (fileType) {
    case File_Types::PATCH_FILE:
        windowTitle += "Patch File";
        extensionFilter = Common_Strings::STRING_PATCH_EXTENSION_FILTER+"\n"+Common_Strings::STRING_ALL_FILE_EXTENSION_FILTER;
        if (save) openLocation = this->settings->defaultPatchSaveLocation;
        else openLocation = this->settings->defaultPatchOpenLocation;
        break;
    case File_Types::QT_CODE_FILE:
        windowTitle += "Qt Code File";
        extensionFilter = Common_Strings::STRING_TEXT_EXTENSION_FILTER+"\n"+Common_Strings::STRING_ALL_FILE_EXTENSION_FILTER;
        if (save) openLocation = this->settings->defaultFileSaveLocation;
        else openLocation = this->settings->defaultFileOpenLocation;
        break;
    case File_Types::ANY_FILE:
        windowTitle += "File";
        extensionFilter = Common_Strings::STRING_ALL_FILE_EXTENSION_FILTER;
        if (save) openLocation = this->settings->defaultFileSaveLocation;
        else openLocation = this->settings->defaultFileOpenLocation;
        break;
    }
    qDebug().noquote() << windowTitle;
    bool isOriginalFile = message == Common_Strings::STRING_ORIGINAL;
    if (isOriginalFile) openLocation = this->settings->defaultOriginalFileOpenLocation;
    if (!QDir(openLocation).exists()) openLocation = this->applicationLocation;
    if (!extension.isEmpty()) extensionFilter = extension.toUpper()+" Files (*."+extension+")\n"+extensionFilter; //add the custom extension if specified

    //Open the Dialog Window
    QString fileLocation = QString();
    if (save) fileLocation = QFileDialog::getSaveFileName(this->parent, windowTitle, openLocation, extensionFilter);
    else fileLocation = QFileDialog::getOpenFileName(this->parent, windowTitle, openLocation, extensionFilter);
    if (fileLocation == nullptr || fileLocation.isEmpty()) return QString();
    QFileInfo fileInfo(fileLocation);
    if (save) {
        if (fileInfo.exists() && !fileInfo.isWritable()) {
            this->errorMessages->Show_Write_Error(fileInfo.fileName());
            return QString();
        }

        //Save the path if the file was saved
        switch (fileType) {
        case File_Types::PATCH_FILE:
            this->settings->defaultPatchSaveLocation = fileInfo.path();
            break;
        case File_Types::QT_CODE_FILE:
        case File_Types::ANY_FILE:
            this->settings->defaultFileSaveLocation = fileInfo.path();
            break;
        }
    } else {
        if (!fileInfo.isReadable()) {
            this->errorMessages->Show_Read_Error(fileInfo.fileName());
            return QString();
        }

        //Save the path if the file was opened
        if (isOriginalFile) {
            this->settings->defaultOriginalFileOpenLocation = fileInfo.path();
        } else {
            switch (fileType) {
            case File_Types::PATCH_FILE:
                this->settings->defaultPatchOpenLocation = fileInfo.path();
                break;
            case File_Types::QT_CODE_FILE:
            case File_Types::ANY_FILE:
                this->settings->defaultFileOpenLocation = fileInfo.path();
                break;
            }
        }
    }
    return fileLocation;
}
