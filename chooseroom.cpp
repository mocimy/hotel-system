#include <QMouseEvent>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include "chooseroom.h"
#include "ui_chooseroom.h"

chooseRoom::chooseRoom(QWidget *parent,int start_time,int end_time) :
    QDialog(parent),
    ui(new Ui::chooseRoom)
{
    ui->setupUi(this);
    QSqlQuery *query = new QSqlQuery();
    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("room");
    model->setFilter("state=0");
    model->select();
    ui->tableWidget->setRowCount(model->rowCount());
    QStringList header;
    header<<"房间号"<<"所在楼层" << "房间类型" << "每日费用";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    for(int i = 0; i<model->rowCount(); ++i){
        QSqlRecord record = model->record(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(record.value("number").toString()));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(record.value("floor").toString()));
        query->exec("SELECT name,price FROM roomtype WHERE id="+record.value("room_type").toString());
        query->next();
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(query->value("name").toString()));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(query->value("price").toString()));
    }
}

chooseRoom::~chooseRoom()
{
    delete ui;
}

void chooseRoom::on_buttonBox_accepted()
{
    QList<QTableWidgetItem *> l = ui->tableWidget->selectedItems();
    emit returnInfo(l.at(0)->text().toInt(),l.at(3)->text().toInt());
    accept();
}
