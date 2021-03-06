#include "Conflicts_Window.h"
#include "ui_Conflicts_Window.h"
#include "Common_Strings.h"

Conflicts_Window::Conflicts_Window(QWidget *parent, const QString &conflictsString, bool conflicts) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Conflicts_Window) {
    ui->setupUi(this);
    ui->tbConflicts->setText(conflictsString);
    if (conflicts) this->setWindowTitle(Common_Strings::STRING_CONFLICTS_DETECTED);
    else this->setWindowTitle(Common_Strings::STRING_COMPATIBLE_FILES_DETECTED);
}

Conflicts_Window::~Conflicts_Window() {
    delete ui;
}

void Conflicts_Window::on_bbOK_accepted() {
    this->close();
}
