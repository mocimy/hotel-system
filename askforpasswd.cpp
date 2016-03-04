#include "askforpasswd.h"
#include "ui_askforpasswd.h"
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QMessageBox>

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

void AskForPasswd::on_buttonBox_accepted()
{
    QString passwd;
    passwd.append(QCryptographicHash::hash(ui->lineEdit->text().toUtf8(), QCryptographicHash::Md5));
    QSqlQuery *query = new QSqlQuery;
    query->exec("SELECT value_text FROM info where name='password'");
    query->next();
    if(passwd==query->value("value_text").toString()) accept();
    else {
        ui->label->setText("密码错误,请重试.");
        ui->label->setStyleSheet("color:red;");
    }
}

void AskForPasswd::on_buttonBox_rejected()
{
    reject();
}
