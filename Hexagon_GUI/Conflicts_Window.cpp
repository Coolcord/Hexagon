#include "Conflicts_Window.h"
#include "ui_Conflicts_Window.h"

Conflicts_Window::Conflicts_Window(QWidget *parent, const QByteArray &conflictBytes) :
    QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
    ui(new Ui::Conflicts_Window) {
    ui->setupUi(this);
    ui->tbConflicts->setText(QString(conflictBytes));
}

Conflicts_Window::~Conflicts_Window() {
    delete ui;
}

void Conflicts_Window::on_bbOK_accepted() {
    this->close();
}
