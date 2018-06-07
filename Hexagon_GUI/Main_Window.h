#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

namespace Ui {
class Main_Window;
}

class Main_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main_Window(QWidget *parent = 0);
    ~Main_Window();

private:
    Ui::Main_Window *ui;
};

#endif // MAIN_WINDOW_H
