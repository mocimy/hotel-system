#include "mainwindow.h"
#include "chooseroom.h"

void MainWindow::initializeCheckIn(){

        on_swithButton_1_clicked();

        /*ui->sex->addItem("男");
           ui->sex->addItem("女");*/

        /*mapper = new QDataWidgetMapper(this);
           mapper->setModel(model);
           mapper->addMapping(ui->costEdit, model->fieldIndex("cost"));
           mapper->addMapping(ui->cost1Edit, model->fieldIndex("cost1"));
           mapper->addMapping(ui->cost2Edit, model->fieldIndex("cost2"));
           mapper->addMapping(ui->cost3Edit, model->fieldIndex("cost3"));
           connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                mapper, SLOT(setCurrentModelIndex(QModelIndex)));*/
}

void MainWindow::on_swithButton_1_clicked()
{
        updateCheckInTable();
        ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_swithButton_2_clicked()
{
        on_daySpinBox_valueChanged(1);
        ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_addItem_clicked()
{
        if(ui->nameEdit->text().isEmpty() ||
           ui->ageEdit->text().isEmpty() ||
           ui->ID_numberEdit->text().isEmpty() ||
           ui->workplaceEdit->text().isEmpty() ||
           ui->cityEdit->text().isEmpty() ||
           ui->roomEdit->text().isEmpty())
                QMessageBox::warning(this,"错误","不能有项目为空。");
        else {
                query->prepare("INSERT INTO hotel (name, sex, age,ID_number,workplace,city,room,start_time_CN,end_time,end_time_CN,cost,cost1,cost2,cost3,deposit,total_deposit)"
                               "VALUES (?, ?, ?, ?, ?, ? ,? ,? ,?,?,?,?,?,?,?,?)");
                query->addBindValue(ui->nameEdit->text());
                query->addBindValue(ui->sexEdit->text());
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
                query->addBindValue(cost*ui->daySpinBox->value());
                query->addBindValue(2*cost*ui->daySpinBox->value());
                if(query->exec()) QMessageBox::information(0,"成功","添加成功!");
                addLog("添加旅客"+ui->nameEdit->text()+"入住");
                query->exec("UPDATE room SET state=1 WHERE state=0 AND number="+QString::number(room_num));
                query->exec("UPDATE room SET state=3 WHERE state=2 AND number="+QString::number(room_num));
                ui->nameEdit->setText("");
                ui->ageEdit->setText("");
                ui->ID_numberEdit->setText("");
                ui->workplaceEdit->setText("");
                ui->cityEdit->setText("");
                ui->roomEdit->setText("");
        }
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
        model->setHeaderData(model->fieldIndex("deposit"),Qt::Horizontal,"需退押金");
        model->setHeaderData(model->fieldIndex("total_deposit"),Qt::Horizontal,"实交押金");
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
        chooseRoom * w = new chooseRoom(this,nowTime,end_time);
        connect(w, &chooseRoom::returnInfo, this, &MainWindow::setRoom);
        w->exec();
}

void MainWindow::setRoom(int roomNum,int price){
        room_num=roomNum;
        cost=price;
        ui->roomEdit->setText(QString::number(roomNum));
        ui->roomEdit_2->setText(QString::number(roomNum));
        ui->priceLabel->setText("价格"+QString::number(cost*ui->doubleSpinBox->value()));
        ui->priceLabel_2->setText("价格"+QString::number(cost*ui->doubleSpinBox->value()));
}

void MainWindow::on_swithButton_3_clicked()
{
        if(ui->tableView->selectionModel()->hasSelection())
        {
                row = ui->tableView->selectionModel()->selectedRows().at(0).row();
                QSqlRecord record =  model->record(row);
                ui->costEdit->setText(record.value("cost").toString());
                ui->cost1Edit->setText(record.value("cost1").toString());
                ui->cost2Edit->setText(record.value("cost2").toString());
                ui->cost3Edit->setText(record.value("cost3").toString());
                ui->stackedWidget->setCurrentIndex(2);
                on_delaySpinBox_valueChanged(ui->delaySpinBox->value());
        }
        else QMessageBox::warning(0,"未选择条目","请在旅客信息界面中选中一个条目。");
}

void MainWindow::on_btnAddCost_clicked()
{
        QSqlRecord record =  model->record(row);
        record.setValue("cost",ui->costEdit->text().toDouble());
        record.setValue("cost1",ui->cost1Edit->text().toDouble());
        record.setValue("cost2",ui->cost2Edit->text().toDouble());
        record.setValue("cost3",ui->cost3Edit->text().toDouble());
        record.setValue("deposit",record.value("total_deposit").toDouble()-ui->costEdit->text().toDouble()
                        -ui->cost1Edit->text().toDouble()
                        -ui->cost2Edit->text().toDouble()
                        -ui->cost3Edit->text().toDouble());
        if(model->setRecord(row,record)) QMessageBox::information(0,"成功","修改成功!");
        /*double res = record.value("total_deposit").toDouble()-ui->costEdit->text().toDouble()
                   -ui->cost1Edit->text().toDouble()
                   -ui->cost2Edit->text().toDouble()
                   -ui->cost3Edit->text().toDouble() ;
           if(mapper->submit()) QMessageBox::information(0,"成功","修改成功!");
           query->exec("UPDATE hotel SET deposit="+QString::number(res)+" WHERE id="+record.value("id").toString());*/
        addLog("修改旅客费用信息");
}

void MainWindow::on_btnLeave_clicked()
{
        QSqlRecord record =  model->record(row);
        model->removeRow(row);
        int cost = record.value("cost").toDouble()+record.value("cost1").toDouble()+record.value("cost2").toDouble()+record.value("cost3").toDouble();
        QMessageBox::information(0,"离店成功","总计消费"+QString::number(cost)+",退还押金"+record.value("deposit").toString());
        addLog("旅客"+record.value("name").toString()+"离店");
        query->exec("UPDATE room SET state=0 WHERE state=1 AND number="+record.value("room").toString());
        query->exec("UPDATE room SET state=2 WHERE state=3 AND number="+record.value("room").toString());
        record.setValue("end_time_CN",timeConvert(nowTime));
        model->setTable("hasleft");
        model->insertRecord(0,record);
        on_swithButton_1_clicked();
}

void MainWindow::on_BtnLeave2_clicked()
{
        QSqlRecord record =  model->record(row);
        model->removeRow(row);
        query->exec("SELECT (value) FROM info WHERE name='preferential'");
        query->next();
        double fav = query->value("value").toDouble()*0.1;
        int cost = record.value("cost").toDouble()*fav+record.value("cost1").toDouble()+record.value("cost2").toDouble()+record.value("cost3").toDouble();
        QMessageBox::information(0,"离店成功","总计消费"+QString::number(cost)+",退还押金"+record.value("deposit").toString());
        addLog("旅客"+record.value("name").toString()+"离店");
        query->exec("UPDATE room SET state=0 WHERE state=1 AND number="+record.value("room").toString());
        query->exec("UPDATE room SET state=2 WHERE state=3 AND number="+record.value("room").toString());
        record.setValue("cost",record.value("cost").toDouble()*fav);
        record.setValue("end_time_CN",timeConvert(nowTime));
        model->setTable("hasleft");
        model->insertRecord(0,record);
        on_swithButton_1_clicked();
}

void MainWindow::on_swithButton_4_clicked()
{
        ui->stackedWidget->setCurrentIndex(3);
        query->exec("SELECT value FROM info WHERE name='floor'");
        query->first();
        int floor = query->value("value").toInt();
        ui->floorBox->clear();
        for(int i = 0; i<floor; ++i)
                ui->floorBox->addItem(QString::number(i+1));
        on_floorBox_currentIndexChanged(0);
        //ui->floorBox->setCurrentIndex(0);
}

void MainWindow::on_floorBox_currentIndexChanged(int index)
{
        if(roomsWidget) ui->floorstack->removeWidget(roomsWidget);
        roomsWidget = new Rooms(this,index+1);
        ui->floorstack->addWidget(roomsWidget);
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
}

void MainWindow::on_daySpinBox_valueChanged(double arg1)
{
        if(int(arg1*10)%10 == 0) end_time = nowTime-(nowTime%1440)+12*60+24*60*arg1;
        else end_time = nowTime-(nowTime%1440)+18*60+24*60*(arg1-0.5);
        ui->endTimeLabel->setText("离店时间为:"+timeConvert(end_time));
        ui->priceLabel->setText("价格"+QString::number(cost*arg1));
}


void MainWindow::on_delaySpinBox_valueChanged(double arg1)
{
        QSqlRecord record =  model->record(row);
        int t = record.value("end_time").toInt();
        if(int(arg1*10)%10 == 0) end_time = t-(t%1440)+12*60+24*60*arg1;
        else end_time = t-(t%1440)+18*60+24*60*(arg1-0.5);
        QSqlTableModel *delayModel = new QSqlTableModel;
        delayModel->setTable("reservation");
        delayModel->setFilter("room="+record.value("room").toString()+" AND start_time<"+QString::number(end_time));
        delayModel->select();
        if(delayModel->rowCount()==0) {
                query->exec("SELECT (room_type) FROM room WHERE number="+record.value("room").toString());
                query->next();
                query->exec("SELECT (price) FROM roomtype WHERE id="+query->value("room_type").toString());
                query->next();
                ui->delayLabel->setText("离店时间为:"+timeConvert(end_time));
                ui->delayPriceLabel->setText(QString::number(arg1*query->value("price").toDouble()));
                ui->delayBtn->setEnabled(true);
        }
        else{
                ui->delayLabel->setText("有其他客户预约，无法延期");
                ui->delayBtn->setDisabled(true);
        }
}

void MainWindow::on_delayBtn_clicked()
{
        QSqlRecord record =  model->record(row);
        record.setValue("end_time",end_time);
        record.setValue("end_time_CN",timeConvert(end_time));
        record.setValue("cost",record.value("cost").toDouble()+ui->delayPriceLabel->text().toDouble());
        if(model->setRecord(row,record))
                QMessageBox::information(this,"成功","成功延期");
}

void MainWindow::on_addManBtn_clicked()
{
        if(ui->sexEdit->text().isEmpty()) ui->sexEdit->setText("男");
        else ui->sexEdit->setText(ui->sexEdit->text()+",男");
}

void MainWindow::on_addWomanBtn_clicked()
{
        if(ui->sexEdit->text().isEmpty()) ui->sexEdit->setText("女");
        else ui->sexEdit->setText(ui->sexEdit->text()+",女");
}
