#include "rooms.h"
#include "ui_rooms.h"
#include <QSqlQuery>
#include <QDebug>
#include <QLabel>

Rooms::Rooms(QWidget *parent,int floor) :
    QWidget(parent),
    ui(new Ui::Rooms)
{
    ui->setupUi(this);
    QSqlQuery *query= new QSqlQuery;
    query->exec("SELECT * FROM room WHERE floor="+QString::number(floor));
    int i = 0 , j = 0;
    while(query->next()){
        QLabel *roomImg = new QLabel();
        roomImg->setPixmap(QPixmap(":images/room"+query->value("room_type").toString()));
        ui->gridLayout_2->addWidget(roomImg,i,j);
        QLabel *roomInfo = new QLabel();
        QString info;
        info.append("房间号"+query->value("number").toString()+'\n');
        info.append("房间类型"+query->value("room_type").toString()+'\n');
        switch(query->value("state").toInt()){
        case 0: info.append("房间状态:空闲"); break;
        case 1: info.append("房间状态:已满"); break;
        case 2: info.append("房间状态:有预约"); break;
        case 3: info.append("房间状态:已满,有预约"); break;
        case 4: info.append("房间状态:维修"); break;
        }
        roomInfo->setText(info);
        ui->gridLayout_2->addWidget(roomInfo,i+1,j);
        ++j ;
        if(j%4 == 0) {
            i=i+2;
            j=0;
        }
    }
}

Rooms::~Rooms()
{
    delete ui;
}
