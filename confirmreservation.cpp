#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include "confirmreservation.h"
#include "ui_confirmreservation.h"

ConfirmReservation::ConfirmReservation(QSqlRecord r,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmReservation)
{
    ui->setupUi(this);
    record=r;
    ui->sex->addItem("男");
    ui->sex->addItem("女");
}

ConfirmReservation::~ConfirmReservation()
{
    delete ui;
}

void ConfirmReservation::on_buttonBox_accepted()
{
    if(ui->ageEdit->text().isEmpty() ||
            ui->ID_numberEdit->text().isEmpty() ||
            ui->workplaceEdit->text().isEmpty() ||
            ui->cityEdit->text().isEmpty())
        QMessageBox::warning(this,"错误","不能有项目为空。");
    else {
        QSqlQuery *query = new QSqlQuery();
        query->exec("SELECT room_type FROM room WHERE number="+record.value("room").toString());
        query->first();
        QString room_type = query->value ("room_type").toString();
        query->exec("SELECT price FROM roomType WHERE id="+room_type);
        query->first();
        int cost = query->value("price").toInt()*((record.value("end_time").toInt()-record.value("start_time").toInt())/(24*60));
        query->prepare("INSERT INTO hotel (name, sex, age,ID_number,workplace,city,room,start_time_CN,end_time,end_time_CN,cost,cost1,cost2,cost3)"
                      "VALUES (?, ?, ?, ?, ?, ? ,? ,? ,?,?,?,?,?,?)");
        query->addBindValue(record.value("name"));
        if(ui->sex->currentIndex()==0) query->addBindValue("男");
        else query->addBindValue("女");
        query->addBindValue(ui->ageEdit->text());
        query->addBindValue(ui->ID_numberEdit->text());
        query->addBindValue(ui->workplaceEdit->text());
        query->addBindValue(ui->workplaceEdit->text());
        query->addBindValue(record.value("room"));
        query->addBindValue(record.value("start_time_CN"));
        query->addBindValue(record.value("end_time"));
        query->addBindValue(record.value("end_time_CN"));
        query->addBindValue(cost);
        query->addBindValue(0);
        query->addBindValue(0);
        query->addBindValue(0);
        query->exec();
    }
}
