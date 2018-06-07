#include "Main_Window_W.h"
#include "ui_Main_Window.h"

Main_Window::Main_Window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Main_Window)
{
    ui->setupUi(this);
}

Main_Window::~Main_Window()
{
    delete ui;
}
