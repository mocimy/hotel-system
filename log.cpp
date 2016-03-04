#include "mainwindow.h"

void MainWindow::initializeLog(){
    on_toolBox_currentChanged(ui->toolBox->currentIndex());
}

void MainWindow::on_searchEdit_2_textChanged(const QString &arg1)
{
    QString search = " like '%"+arg1+"%' ";
    if(ui->toolBox->currentIndex()==0){
        model->setTable("log");
        model->setFilter("time"+search+"OR content"+search);
        //qDebug() << "time"+search+"OR content"+search;
        if (!model->select()) {
            showError(model->lastError());
            return;
        }
        ui->listWidget->clear();
        //qDebug() << model->rowCount();
        for(int i = 0; i<model->rowCount(); ++i){
            QSqlRecord record = model->record(i);
            ui->listWidget->addItem(record.value("time").toString()+"               "+record.value("content").toString());
        }
    }
    else {
        model->setTable("hasleft");
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
}

void MainWindow::addLog(QString log){
    query->exec("INSERT INTO log (time,content) VALUES ( datetime(),'"+log+"')");
}

void MainWindow::on_toolBox_currentChanged(int index)
{
    if(index==0) {
        ui->listWidget->clear();
        model->setTable("log");
        model->select();
        for(int i = 0; i<model->rowCount(); ++i){
            QSqlRecord record = model->record(i);
            ui->listWidget->addItem(record.value("time").toString()+"               "+record.value("content").toString());
        }
    }
    else {
        model->setTable("hasleft");
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
        ui->hasLeftView->setModel(model);
        ui->hasLeftView->setColumnHidden(model->fieldIndex("id"),true);
        ui->hasLeftView->setColumnHidden(model->fieldIndex("end_time"),true);
    }
}
