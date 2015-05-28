#include "mainwindow.h"
#include "chooseroom.h"

void MainWindow::initializeCheckIn(){
    ui->stackedWidget->setCurrentIndex(0);
    //初始化选择框
    ui->sex->addItem("男");
    ui->sex->addItem("女");
    updateCheckInTable();
    query->exec("SELECT value FROM info WHERE name='floor'");
    query->first();
    int floor = query->value("value").toInt();
    for(int i = 0;i<floor;++i)
        ui->floorBox->addItem(QString::number(i+1));
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->addMapping(ui->lineEdit_8, model->fieldIndex("cost1"));
    mapper->addMapping(ui->lineEdit_9, model->fieldIndex("cost2"));
    mapper->addMapping(ui->lineEdit_10, model->fieldIndex("cost3"));
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            mapper, SLOT(setCurrentModelIndex(QModelIndex)));
}

void MainWindow::on_swithButton_1_clicked()
{
    updateCheckInTable();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_swithButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_addItem_clicked()
{
    query->prepare("INSERT INTO hotel (name, sex, age,ID_number,workplace,city,room,start_time_CN,end_time,end_time_CN,cost,cost1,cost2,cost3)"
                  "VALUES (?, ?, ?, ?, ?, ? ,? ,? ,?,?,?,?,?,?)");
    query->addBindValue(ui->nameEdit->text());
    if(ui->sex->currentIndex()==0) query->addBindValue("男");
    else query->addBindValue("女");
    query->addBindValue(ui->ageEdit->text());
    query->addBindValue(ui->ID_numberEdit->text().toInt());
    query->addBindValue(ui->workplaceEdit->text());
    query->addBindValue(ui->cityEdit->text());
    query->addBindValue(ui->roomEdit->text().toInt());
    query->addBindValue(timeConvert(nowTime));
    query->addBindValue(end_time);
    query->addBindValue(timeConvert(end_time));
    query->addBindValue(cost*ui->daySpinBox->value());
    query->addBindValue(0);
    query->addBindValue(0);
    query->addBindValue(0);
    if(query->exec()) QMessageBox::information(0,"成功","添加成功!");
    addLog("添加旅客'"+ui->nameEdit->text()+"'入住");
    query->exec("UPDATE room SET state=1 WHERE number="+QString::number(room_num));
    ui->nameEdit->setText("");
    ui->ageEdit->setText("");
    ui->ID_numberEdit->setText("");
    ui->workplaceEdit->setText("");
    ui->cityEdit->setText("");
    ui->roomEdit->setText("");
}

void MainWindow::updateCheckInTable(){
    model->setTable("hotel");
    model->setHeaderData(model->fieldIndex("name"),Qt::Horizontal,"姓名");
    model->setHeaderData(model->fieldIndex("sex"),Qt::Horizontal,"性别");
    model->setHeaderData(model->fieldIndex("age"),Qt::Horizontal,"年龄");
    model->setHeaderData(model->fieldIndex("ID_number"),Qt::Horizontal,"身份证号码");
    model->setHeaderData(model->fieldIndex("workplace"),Qt::Horizontal,"工作单位");
    model->setHeaderData(model->fieldIndex("city"),Qt::Horizontal,"城市");
    model->setHeaderData(model->fieldIndex("start_time_CN"),Qt::Horizontal,"入住时间");
    model->setHeaderData(model->fieldIndex("end_time_CN"),Qt::Horizontal,"离店时间");
    model->setHeaderData(model->fieldIndex("room"),Qt::Horizontal,"房间号");
    model->setHeaderData(model->fieldIndex("cost"),Qt::Horizontal,"住宿费");
    model->setHeaderData(model->fieldIndex("cost1"),Qt::Horizontal,"食品消耗费");
    model->setHeaderData(model->fieldIndex("cost2"),Qt::Horizontal,"通信费");
    model->setHeaderData(model->fieldIndex("cost3"),Qt::Horizontal,"损坏赔偿费");
    if (!model->select()) {
        showError(model->lastError());
        return;
    }
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(model->fieldIndex("id"),true);
    ui->tableView->setColumnHidden(model->fieldIndex("end_time"),true);
}

void MainWindow::on_chooseRoom_clicked()
{
    chooseRoom * w = new chooseRoom();
    connect(w, &chooseRoom::returnInfo, this, &MainWindow::setRoom);
    w->exec();
}

void MainWindow::setRoom(int roomNum,int price){
    room_num=roomNum;
    cost=price;
    ui->roomEdit->setText(QString::number(roomNum));
}

void MainWindow::on_swithButton_3_clicked()
{
    if(ui->tableView->selectionModel()->hasSelection())
        ui->stackedWidget->setCurrentIndex(2);
    else QMessageBox::warning(0,"未选择条目","请在旅客信息界面中选中一个条目。");
}

void MainWindow::on_btnAddCost_clicked()
{
    if(mapper->submit()) QMessageBox::information(0,"成功","修改成功!");
}

void MainWindow::on_btnLeave_clicked()
{
    int row = ui->tableView->selectionModel()->selectedRows().at(0).row();
    QSqlRecord record =  model->record(row);
    model->removeRow(row);
    int cost = record.value("cost").toInt()+record.value("cost1").toInt()+record.value("cost2").toInt()+record.value("cost3").toInt();
    QMessageBox::information(0,"离店成功","总计消费"+QString::number(cost));
    query->exec("UPDATE room SET state=0 WHERE number="+record.value("room").toString());
    record.setValue("end_time_CN",timeConvert(nowTime));
    model->setTable("hasleft");
    model->insertRecord(0,record);
}

void MainWindow::on_swithButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    on_floorBox_currentIndexChanged(0);
}

void MainWindow::on_floorBox_currentIndexChanged(int index)
{
    ui->floorstack->removeWidget(roomsWidget[index]);
    roomsWidget[index] = new Rooms(this,index+1);
    ui->floorstack->insertWidget(index,roomsWidget[index]);
    ui->floorstack->setCurrentIndex(index);
}


void MainWindow::on_searchEdit_textChanged(const QString &arg1)
{
    QString search = " like '%"+arg1+"%' ";
    model->setFilter("name"+search+
                     "OR sex"+search+
                     "OR age"+search+
                     "OR ID_number"+search+
                     "OR workplace"+search+
                     "OR city"+search+
                     "OR room"+search+
                     "OR start_time_CN"+search+
                     "OR end_time_CN"+search+
                     "OR cost"+search+
                     "OR cost1"+search+
                     "OR cost2"+search+
                     "OR cost3"+search);
    if (!model->select()) {
        showError(model->lastError());
        return;
    }
    ui->tableView->setModel(model);
}


void MainWindow::on_daySpinBox_valueChanged(double arg1)
{
    if(int(arg1*10)%10 == 0) end_time = nowTime-(nowTime%1440)+12*60+24*60*arg1;
    else end_time = nowTime-(nowTime%1440)+18*60+24*60*(arg1-0.5);
    /*if( (nowTime%1440) >= 0 && (nowTime%1440) <12*60){
        if(int(arg1*10)%10 == 0) end_time=nowTime-(nowTime%1440)-6*60+1440*arg1;
        else end_time=nowTime-(nowTime%1440)+12*60+1440*(arg1-0.5) ;
    }
    else if( (nowTime%1440) >= 12*60 && (nowTime%1440) <18*60){
        if(int(arg1*10)%10 == 0) end_time=nowTime-(nowTime%1440)+12*60+1440*arg1;
        else end_time=nowTime-(nowTime%1440)+18*60+1440*(arg1-0.5) ;
    }
    else{
        if(int(arg1*10)%10 == 0) end_time=nowTime-(nowTime%1440)+18*60+1440*arg1;
        else end_time=nowTime-(nowTime%1440)+36*60+1440*(arg1-0.5) ;
    }*/
    ui->endTimeLabel->setText("离店时间为:"+timeConvert(end_time));
}
