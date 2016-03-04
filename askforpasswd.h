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

private:
    Ui::AskForPasswd *ui;
};

#endif // ASKFORPASSWD_H
