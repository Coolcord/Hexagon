#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QDialog>

namespace Ui {
class Main_Window;
}

class Main_Window : public QDialog
{
    Q_OBJECT

public:
    explicit Main_Window(QWidget *parent = 0);
    ~Main_Window();

private slots:
    void on_btnApplyPatch_clicked();
    void on_btnCreatePatch_clicked();
    void on_btnCheckAgainstPatch_clicked();
    void on_btnCheckAgainstFolder_clicked();
    void on_btnConvertHEXPtoQtCode_clicked();
    void on_btnConvertQtCodetoHEXP_clicked();
    void on_leOriginalFile_textChanged(const QString &arg1);
    void on_tbOriginalFile_clicked();
    void on_sbCompareSize_valueChanged(int arg1);
    void on_cbAskForSaveLocation_clicked(bool checked);

private:
    Ui::Main_Window *ui;
};

#endif // MAIN_WINDOW_H
