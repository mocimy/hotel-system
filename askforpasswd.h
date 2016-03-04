#ifndef ASKFORPASSWD_H
#define ASKFORPASSWD_H

#include <QDialog>

namespace Ui {
class AskForPasswd;
}

class AskForPasswd : public QDialog
{
    Q_OBJECT

public:
    explicit AskForPasswd(QWidget *parent = 0);
    ~AskForPasswd();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AskForPasswd *ui;
};

#endif // ASKFORPASSWD_H
