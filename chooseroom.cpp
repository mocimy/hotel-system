#include <QMouseEvent>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include "chooseroom.h"
#include "ui_chooseroom.h"

/*chooseRoom::chooseRoom(QWidget *parent,int start_time,int end_time) :
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
}*/

chooseRoom::chooseRoom(QWidget *parent,int start_time,int end_time) :
    QDialog(parent),
    ui(new Ui::chooseRoom)
{
    ui->setupUi(this);
    startTime = start_time;
    endTime = end_time;
    //qDebug() << start_time <<" "<<end_time;
    QSqlQuery *query = new QSqlQuery();
    //QSqlTableModel *model = new QSqlTableModel();
    query->exec("SELECT (name) FROM roomType");
    ui->comboBox->addItem("全部");
    while (query->next()) {
        ui->comboBox->addItem(query->value("name").toString());
    }
    /*model->setTable("room");
    model->select();
    ui->tableWidget->setRowCount(model->rowCount());
    QStringList header;
    header<<"房间号"<<"所在楼层" << "房间类型" << "每日费用";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    int j = 0 ;
    for(int i = 0; i<model->rowCount(); ++i){
        QSqlRecord record = model->record(i);
        if(record.value("state").toInt()==4) continue;
        else if(record.value("state").toInt()!=0){
            query->exec("SELECT end_time FROM hotel WHERE room="+record.value("number").toString());
            if(query->first() && query->value("end_time").toInt()>start_time) {
                //qDebug() << query->value("end_time").toInt();
                continue;
            }
            query->exec("SELECT  * FROM reservation WHERE room="+record.value("number").toString());
            bool ok = true;
            while(ok && query->next())
                if(!( query->value("start_time")>end_time || query->value("end_time")<start_time )) ok=false;
            if(!ok) continue;
        }
        ui->tableWidget->setItem(j,0,new QTableWidgetItem(record.value("number").toString()));
        ui->tableWidget->setItem(j,1,new QTableWidgetItem(record.value("floor").toString()));
        query->exec("SELECT * FROM roomtype WHERE id="+record.value("room_type").toString());
        //qDebug() << query->lastError();
        query->next();
        ui->tableWidget->setItem(j,2,new QTableWidgetItem(query->value("name").toString()));
        ui->tableWidget->setItem(j,3,new QTableWidgetItem(query->value("price").toString()));
        ++j;
    }
    ui->tableWidget->setRowCount(j);*/
    on_comboBox_currentIndexChanged("全部");
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

void chooseRoom::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QSqlQuery *query = new QSqlQuery();
    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("room");
    if(arg1!="全部") {
        query->exec("SELECT (id) FROM roomType WHERE name = '"+arg1+"'");
        query->next();
        model->setFilter("room_type="+query->value("id").toString());
    }
    model->select();
    ui->tableWidget->setRowCount(model->rowCount());
    QStringList header;
    header<<"房间号"<<"所在楼层" << "房间类型" << "每日费用";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    int j = 0 ;
    for(int i = 0; i<model->rowCount(); ++i){
        QSqlRecord record = model->record(i);
        if(record.value("state").toInt()==4) continue;
        else if(record.value("state").toInt()!=0){
            query->exec("SELECT end_time FROM hotel WHERE room="+record.value("number").toString());
            if(query->first() && query->value("end_time").toInt()>startTime) {
                //qDebug() << query->value("end_time").toInt();
                continue;
            }
            query->exec("SELECT  * FROM reservation WHERE room="+record.value("number").toString());
            bool ok = true;
            while(ok && query->next())
                if(!( query->value("start_time")>endTime || query->value("end_time")<startTime )) ok=false;
            if(!ok) continue;
        }
        ui->tableWidget->setItem(j,0,new QTableWidgetItem(record.value("number").toString()));
        ui->tableWidget->setItem(j,1,new QTableWidgetItem(record.value("floor").toString()));
        query->exec("SELECT * FROM roomtype WHERE id="+record.value("room_type").toString());
        //qDebug() << query->lastError();
        query->next();
        ui->tableWidget->setItem(j,2,new QTableWidgetItem(query->value("name").toString()));
        ui->tableWidget->setItem(j,3,new QTableWidgetItem(query->value("price").toString()));
        ++j;
    }
    ui->tableWidget->setRowCount(j);
}
