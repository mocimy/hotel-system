#include "mainwindow.h"
#include "chooseroom.h"
#include "confirmreservation.h"

void MainWindow::initializeReserve(){
    on_confirmReservationBtn_clicked();
}

void MainWindow::on_confirmReservationBtn_clicked()
{
    model->setTable("reservation");
    model->setFilter("start_time<="+QString::number(nowTime)+" AND start_time>="+QString::number(nowTime-240));
    model->setHeaderData(model->fieldIndex("name"),Qt::Horizontal,"姓名");
    model->setHeaderData(model->fieldIndex("telphone"),Qt::Horizontal,"联系方式");
    model->setHeaderData(model->fieldIndex("reservation_time_CN"),Qt::Horizontal,"预约时间");
    model->setHeaderData(model->fieldIndex("start_time_CN"),Qt::Horizontal,"入住时间");
    model->setHeaderData(model->fieldIndex("end_time_CN"),Qt::Horizontal,"预计离店时间");
    model->setHeaderData(model->fieldIndex("room"),Qt::Horizontal,"预约房间号");
    model->select();
    ui->reserveView->setModel(model);
    ui->reserveView->setColumnHidden(model->fieldIndex("id"),true);
    ui->reserveView->setColumnHidden(model->fieldIndex("start_time"),true);
    ui->reserveView->setColumnHidden(model->fieldIndex("end_time"),true);
    ui->stackedWidget_2->setCurrentIndex(0);
}
void MainWindow::on_newReservationBtn_clicked()
{
    ui->DataSpinBox->setMinimum(nowTime/(24*60));
    ui->DataSpinBox->setMaximum(nowTime/(24*60)+30);
    //ui->dateEdit->setText(QString::number(nowTime/(24*60)));
    on_timeEdit_timeChanged(ui->timeEdit->time());
    on_doubleSpinBox_valueChanged(ui->doubleSpinBox->value());
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_allReservationBtn_clicked()
{
    model->setTable("reservation");
    model->setHeaderData(model->fieldIndex("name"),Qt::Horizontal,"姓名");
    model->setHeaderData(model->fieldIndex("telphone"),Qt::Horizontal,"联系方式");
    model->setHeaderData(model->fieldIndex("reservation_time_CN"),Qt::Horizontal,"预约时间");
    model->setHeaderData(model->fieldIndex("start_time_CN"),Qt::Horizontal,"入住时间");
    model->setHeaderData(model->fieldIndex("end_time_CN"),Qt::Horizontal,"预计离店时间");
    model->setHeaderData(model->fieldIndex("room"),Qt::Horizontal,"预约房间号");
    model->select();
    ui->allReserveView->setModel(model);
    ui->allReserveView->setColumnHidden(model->fieldIndex("id"),true);
    ui->allReserveView->setColumnHidden(model->fieldIndex("start_time"),true);
    ui->allReserveView->setColumnHidden(model->fieldIndex("end_time"),true);
    ui->stackedWidget_2->setCurrentIndex(2);
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    int start_date = ui->DataSpinBox->value()*24*60;
    if(int(arg1*10)%10 == 0) end_time = start_date+12*60+24*60*arg1;
    else end_time = start_date+18*60+24*60*(arg1-0.5);
    ui->endTimeLabel_2->setText("离店时间为:"+timeConvert(end_time));
    ui->priceLabel_2->setText("价格"+QString::number(cost*arg1));
}

void MainWindow::on_submitBtn_clicked()
{
    query->prepare("INSERT INTO reservation (name,telphone,reservation_time_CN,start_time,start_time_CN,end_time,end_time_CN,room) VALUES (?,?,?,?,?,?,?,?)");
    query->addBindValue(ui->nameEdit_2->text());
    query->addBindValue(ui->telEdit->text());
    query->addBindValue(timeConvert(nowTime));
    query->addBindValue(start_time);
    query->addBindValue(timeConvert(start_time));
    query->addBindValue(end_time);
    query->addBindValue(timeConvert(end_time));
    query->addBindValue(room_num);
    if(query->exec())
        QMessageBox::information(this,"成功","添加成功");
    addLog("添加旅客"+ui->nameEdit_2->text()+"预约信息");
    //qDebug() << query->lastError();
    query->exec("UPDATE room SET state=2 WHERE state=0 AND number="+QString::number(room_num));
    query->exec("UPDATE room SET state=3 WHERE state=1 AND number="+QString::number(room_num));
    ui->nameEdit_2->setText("");
    ui->telEdit->setText("");
    ui->roomEdit_2->setText("");
}

void MainWindow::on_chooseRoom_2_clicked()
{
    chooseRoom * w = new chooseRoom(this,start_time,end_time);
    connect(w, &chooseRoom::returnInfo, this, &MainWindow::setRoom);
    w->exec();
}

void MainWindow::on_timeEdit_timeChanged(const QTime &time)
{
    start_time = ui->DataSpinBox->value()*24*60+time.hour()*60+time.minute();
}


void MainWindow::on_DataSpinBox_valueChanged(int arg1)
{
    start_time = arg1*24*60+ui->timeEdit->time().hour()*60+ui->timeEdit->time().minute();
    on_doubleSpinBox_valueChanged(ui->doubleSpinBox->value());
}

void MainWindow::on_confirmBtn_clicked()
{
    row = ui->reserveView->selectionModel()->selectedRows().at(0).row();
    QSqlRecord record =  model->record(row);
    w = new ConfirmReservation(record,this);
    ui->horizontalLayout_4->addWidget(w);
    connect(w, &ConfirmReservation::accepted, this, &MainWindow::acceptConfirm);
    connect(w, &ConfirmReservation::rejected, this, &MainWindow::rejectConfirm);
}

void MainWindow::acceptConfirm(){
    QSqlTableModel *hasReservationModel = new QSqlTableModel;
    hasReservationModel->setTable("reservation");
    hasReservationModel->setFilter("room="+model->record(row).value("room").toString());
    hasReservationModel->select();
    if(hasReservationModel->rowCount()==1) query->exec("UPDATE room SET state=1 WHERE number="+model->record(row).value("room").toString());
    else query->exec("UPDATE room SET state=3 WHERE number="+model->record(row).value("room").toString());
    addLog("已预约旅客"+model->record(row).value("name").toString()+"确认入住");
    model->removeRow(row);
    model->select();
    ui->horizontalLayout->removeWidget(w);
}

void MainWindow::rejectConfirm(){
    ui->horizontalLayout->removeWidget(w);
}

void MainWindow::on_searchEdit_3_textChanged(const QString &arg1)
{
    QString search = " like '%"+arg1+"%' ";
    model->setFilter("start_time<="+QString::number(nowTime)+" AND start_time>="+QString::number(nowTime-240)+
                     " AND (name"+search+
                     "OR telphone"+search+
                     "OR start_time_CN"+search+
                     "OR end_time_CN"+search+
                     "OR room"+search+")");
    if (!model->select()) {
        showError(model->lastError());
        return;
    }
}

void MainWindow::on_searchEdit_4_textChanged(const QString &arg1)
{
    if(ui->reserveView->selectionModel()->hasSelection()) {
        QString search = " like '%"+arg1+"%' ";
        model->setFilter("name"+search+
                         "OR telphone"+search+
                         "OR start_time_CN"+search+
                         "OR end_time_CN"+search+
                         "OR room"+search);
        if (!model->select()) {
            showError(model->lastError());
            return;
        }
    }
    else QMessageBox::warning(this,"错误","未选择条目!");
}


void MainWindow::on_submitChangeBtn_clicked()
{
    model->submitAll();
    addLog("修改预约旅客信息");
}

void MainWindow::on_delItemBtn_clicked()
{
    if(ui->reserveView->selectionModel()->hasSelection()) {
        row = ui->reserveView->selectionModel()->selectedRows().at(0).row();
        QSqlTableModel *hasReservationModel = new QSqlTableModel;
        hasReservationModel->setTable("reservation");
        hasReservationModel->setFilter("room="+model->record(row).value("room").toString());
        hasReservationModel->select();
        if(hasReservationModel->rowCount()==1) {
            query->exec("UPDATE room SET state=1 WHERE state=3 AND number="+model->record(row).value("room").toString());
            query->exec("UPDATE room SET state=0 WHERE state=2 AND number="+model->record(row).value("room").toString());
        }
        model->removeRow(row);
        model->select();
        addLog("删除旅客"+model->record(row).value("name").toString()+"预约信息");
    }
    else QMessageBox::warning(this,"错误","未选择条目!");
}

void MainWindow::on_submitChangeBtn_2_clicked()
{
    model->submitAll();
    addLog("修改预约旅客信息");
}

void MainWindow::on_delItemBtn_2_clicked()
{
    if(ui->allReserveView->selectionModel()->hasSelection()) {
        row = ui->allReserveView->selectionModel()->selectedRows().at(0).row();
        QSqlTableModel *hasReservationModel = new QSqlTableModel;
        hasReservationModel->setTable("reservation");
        hasReservationModel->setFilter("room="+model->record(row).value("room").toString());
        hasReservationModel->select();
        if(hasReservationModel->rowCount()==1) {
            query->exec("UPDATE room SET state=1 WHERE state=3 AND number="+model->record(row).value("room").toString());
            query->exec("UPDATE room SET state=0 WHERE state=2 AND number="+model->record(row).value("room").toString());
        }
        model->removeRow(row);
        model->select();
        addLog("删除旅客"+model->record(row).value("name").toString()+"预约信息");
    }
    else QMessageBox::warning(this,"错误","未选择条目!");
}
