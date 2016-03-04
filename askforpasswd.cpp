#include "askforpasswd.h"
#include "ui_askforpasswd.h"

AskForPasswd::AskForPasswd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskForPasswd)
{
    ui->setupUi(this);
}

AskForPasswd::~AskForPasswd()
{
    delete ui;
}
