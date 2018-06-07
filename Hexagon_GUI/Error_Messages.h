#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H

#include <QString>
#include <QWidget>

class Error_Messages {
public:
    Error_Messages();
    Error_Messages(QWidget *parent);
    ~Error_Messages() {}
    void Show_Information(const QString &message);
    void Show_Error(const QString &message);
    void Show_Parse_Error(int lineNum);
    void Show_Read_Error(const QString &fileName);
    void Show_Write_Error(const QString &fileName);
    void Show_Read_Write_Error(const QString &fileName);
    void Update_Parent(QWidget *parent);

private:
    void Show_Read_Write_Error(const QString &fileName, const QString &mode);

    QWidget *parent;
};

#endif // ERROR_MESSAGES_H
