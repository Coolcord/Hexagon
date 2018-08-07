#include "Error_Messages.h"
#include "Common_Strings.h"
#include <assert.h>
#include <QMessageBox>

Error_Messages::Error_Messages() {
    this->parent = nullptr;
}

Error_Messages::Error_Messages(QWidget *parent) {
    assert(parent);
    this->parent = parent;
}

void Error_Messages::Show_Information(const QString &message) {
    QMessageBox::information(this->parent, Common_Strings::STRING_HEXAGON, message, Common_Strings::STRING_OK);
}

void Error_Messages::Show_Error(const QString &message) {
    QMessageBox::critical(this->parent, Common_Strings::STRING_HEXAGON, message, Common_Strings::STRING_OK);
}

void Error_Messages::Show_Parse_Error(int lineNum) {
    return this->Show_Parse_Error("patch", lineNum);
}

void Error_Messages::Show_Parse_Error(const QString &fileName, int lineNum) {
    return this->Show_Error("Parse error in "+fileName+" on line "+QString::number(lineNum));
}

void Error_Messages::Show_Read_Error(const QString &fileName) {
    return this->Show_Read_Write_Error(fileName, "read");
}

void Error_Messages::Show_Write_Error(const QString &fileName) {
    return this->Show_Read_Write_Error(fileName, "write");
}

void Error_Messages::Show_Read_Write_Error(const QString &fileName) {
    return this->Show_Read_Write_Error(fileName, "read/write");
}

void Error_Messages::Update_Parent(QWidget *parent) {
    this->parent = parent;
}

void Error_Messages::Show_Read_Write_Error(const QString &fileName, const QString &mode) {
    return this->Show_Error("Unable to "+mode+" "+fileName+"!");
}
