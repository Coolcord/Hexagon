#ifndef CONFLICTS_WINDOW_H
#define CONFLICTS_WINDOW_H

#include <QByteArray>
#include <QDialog>

namespace Ui {
class Conflicts_Window;
}

class Conflicts_Window : public QDialog {
    Q_OBJECT

public:
    Conflicts_Window(QWidget *parent, const QString &conflictsString, bool conflicts);
    ~Conflicts_Window();

private slots:
    void on_bbOK_accepted();

private:
    Ui::Conflicts_Window *ui;
};

#endif // CONFLICTS_WINDOW_H
