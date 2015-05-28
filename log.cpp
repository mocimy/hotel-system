#include "mainwindow.h"

void MainWindow::initializeLog(){
    on_searchEdit_2_textChanged("");
}

void MainWindow::on_searchEdit_2_textChanged(const QString &arg1)
{
    QString search = " like '%"+arg1+"%' ";
    QSqlTableModel * logModel = new QSqlTableModel;
    logModel->setTable("log");
    logModel->setFilter("time"+search+"OR content"+search);
    if (!logModel->select()) {
        showError(logModel->lastError());
        return;
    }
    ui->logView->setModel(logModel);
    QSqlTableModel * hasLeftModel = new QSqlTableModel;
    hasLeftModel->setTable("hasleft");
    hasLeftModel->setFilter("name"+search+
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
    if (!hasLeftModel->select()) {
        showError(hasLeftModel->lastError());
        return;
    }
    ui->hasLeftView->setModel(hasLeftModel);
}

void MainWindow::addLog(QString log){
    query->exec("INSERT INTO log (time,content) VALUES ( datetime(),'"+log+"')");
}
