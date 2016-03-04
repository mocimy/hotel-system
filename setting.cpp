#include "mainwindow.h"
#include "qcustomplot.h"

void MainWindow::initializeSetting(){
    query->exec("SELECT value FROM info WHERE name='password'");
    query->next();
    if(query->value("value").toInt()==0) {
        ui->passwordCheckBox->setChecked(false);
        on_passwordCheckBox_stateChanged(0);
    }
    else {
        on_passwordCheckBox_stateChanged(1);
        ui->passwordCheckBox->setChecked(true);
    }
    query->exec("SELECT value FROM info WHERE name='floor'");
    query->next();
    ui->floorEdit->setText(query->value("value").toString());
    query->exec("SELECT value FROM info WHERE name='preferential'");
    query->next();
    ui->favSpinBox->setValue(query->value("value").toInt());
    ui->skinBox->addItem("炫酷黑");
    ui->skinBox->addItem("经典白");
    QDir dir ;
    if(!dir.exists("backup")) dir.mkdir("backup");
    dir.cd("backup");
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Time);
    QFileInfoList list = dir.entryInfoList();
    ui->backupBox->clear();
    for (int i = 0; i < list.size(); ++i) {
        ui->backupBox->addItem(list.at(i).fileName());
    }

    refresh();

    roomModel = new QSqlTableModel;
    roomTypeModel = new QSqlTableModel;
    roomModel->setTable("room");
    roomTypeModel->setTable("roomtype");
    roomModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    roomTypeModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    roomModel->setHeaderData(roomModel->fieldIndex("number"),Qt::Horizontal,"房间号");
    roomModel->setHeaderData(roomModel->fieldIndex("room_type"),Qt::Horizontal,"房间种类");
    roomModel->setHeaderData(roomModel->fieldIndex("floor"),Qt::Horizontal,"所在楼层");
    roomTypeModel->setHeaderData(roomTypeModel->fieldIndex("id"),Qt::Horizontal,"序号");
    roomTypeModel->setHeaderData(roomTypeModel->fieldIndex("name"),Qt::Horizontal,"名称");
    roomTypeModel->setHeaderData(roomTypeModel->fieldIndex("price"),Qt::Horizontal,"价格/天");
    roomModel->select();
    roomTypeModel->select();
    ui->roomView->setModel(roomModel);
    ui->roomTypeView->setModel(roomTypeModel);
    ui->roomView->setColumnHidden(1,true);

    customPlot = new QCustomPlot;
    ui->verticalLayout_5->addWidget(customPlot);
}


void MainWindow::on_passwordCheckBox_stateChanged(int arg1)
{
    if(arg1==0) {
        query->exec("UPDATE info SET value=0 WHERE name='password'");
        ui->passwordEdit->setEnabled(false);
    }
    else {
        query->exec("UPDATE info SET value=1 WHERE name='password'");
        ui->passwordEdit->setEnabled(true);
    }
}

void MainWindow::on_favSpinBox_valueChanged(const QString &arg1)
{
    query->exec("UPDATE info SET value="+arg1+" WHERE name='preferential'");
}

void MainWindow::on_passwordEdit_editingFinished()
{
    QString passwd;
    passwd.append(QCryptographicHash::hash(ui->passwordEdit->text().toUtf8(), QCryptographicHash::Md5));
    query->exec("UPDATE info SET value_text='"+passwd+"' WHERE name='password'");
}

void MainWindow::on_backupBtn_clicked()
{
    query->exec("UPDATE info SET value="+QString::number(nowTime)+" WHERE name='time'");
    if(QFile::copy("data.db","backup/"+ui->backupNameEdit->text()))
        QMessageBox::information(this,"成功","备份成功");
    addLog("创建新备份: "+ui->backupNameEdit->text());
    ui->backupBox->addItem(ui->backupNameEdit->text());
}

void MainWindow::on_restoreBtn_clicked()
{
    db.close();
    QFile file("data.db");
    if(file.remove() && QFile::copy("backup/"+ui->backupBox->currentText(),"data.db"))
        QMessageBox::information(this,"成功","恢复成功");
    sqlite();
    /*db.close();
    db.open();*/
    nowTime=getTime();
    ui->label->setText(timeConvert(nowTime));
    addLog("恢复到备份:"+ui->backupBox->currentText());
    initializeSetting();
}

void MainWindow::on_delBackupBtn_clicked()
{
    QFile file("backup/"+ui->backupBox->currentText());
    if(file.remove()) QMessageBox::information(this,"成功","删除成功");
    addLog("删除备份:"+ui->backupBox->currentText());
    ui->backupBox->removeItem(ui->backupBox->currentIndex());
}

void MainWindow::on_floorEdit_textChanged(const QString &arg1)
{
    query->exec("UPDATE info SET value="+arg1+" WHERE name='floor'");
}

void MainWindow::on_roomEditBtn_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(1);
}

void MainWindow::on_goBackBtn_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
}

void MainWindow::on_addRoomTypeBtn_clicked()
{
    row = roomTypeModel->rowCount();
    roomTypeModel->insertRow(row);
    roomTypeModel->setData(roomTypeModel->index(row, 0), roomTypeModel->record(row-1).value("id").toInt()+1);
}

void MainWindow::on_delRoomTypeBtn_clicked()
{
    if(ui->roomTypeView->selectionModel()->hasSelection()){
        roomTypeModel->removeRow(ui->roomTypeView->selectionModel()->selectedRows().at(0).row());
        roomTypeModel->select();
    }
    else QMessageBox::warning(this,"错误","未选择条目!");
}

void MainWindow::on_addroomBtn_clicked()
{
    row = roomModel->rowCount();
    roomModel->insertRow(row);
}

void MainWindow::on_delroomBtn_clicked()
{
    if(ui->roomView->selectionModel()->hasSelection()){
        roomModel->removeRow(ui->roomView->selectionModel()->selectedRows().at(0).row());
        roomModel->select();
    }
    else QMessageBox::warning(this,"错误","未选择条目!");
}

void MainWindow::on_sortBtn_clicked()
{
    roomModel->sort(roomModel->fieldIndex("number"),Qt::AscendingOrder);
}

void MainWindow::on_skinBox_currentIndexChanged(int index)
{
    if(index==1) app->setStyleSheet("");
    else {
        QFile f(":qdarkstyle/style.qss");
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app->setStyleSheet(ts.readAll());
    }
}

void MainWindow::on_fontComboBox_currentFontChanged(const QFont &font)
{
    if(ui->skinBox->currentIndex()==1) app->setFont(font);
    else {
        QFile f(":qdarkstyle/style.qss");
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app->setStyleSheet(ts.readAll()+"*{font-family:\""+font.family()+"\";}");
    }
}

void MainWindow::on_reinitializeBtn_clicked()
{
    QFile file("data.db");
    file.remove();
    sqlite();
    initializeSetting();
}

void MainWindow::on_goBackBtn_2_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
}

void MainWindow::on_detailBtn_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(2);
    on_statisticsBox_currentIndexChanged(ui->statisticsBox->currentIndex());
}

void MainWindow::refresh()
{
    QSqlTableModel * statisticsModel = new QSqlTableModel();
    statisticsModel->setTable("room");
    statisticsModel->select();
    double total = statisticsModel->rowCount();
    statisticsModel->setFilter("state=1 OR state=3");
    statisticsModel->select();
    double live = statisticsModel->rowCount();
    statisticsModel->setFilter("state=2 OR state=4");
    statisticsModel->select();
    double reserve = statisticsModel->rowCount();
    ui->checkRatesLabel->setText(QString::number(live/total));
    ui->bookingRatesLabel->setText(QString::number(reserve/total));
    query->exec("SELECT sum(cost),sum(cost1),sum(cost2) FROM hasleft");
    query->next();
    ui->revenueLabel->setText(QString::number(
                                  query->value(0).toDouble()+
                                  query->value(1).toDouble()+
                                  query->value(2).toDouble() ));
}

void MainWindow::on_statisticsBox_currentIndexChanged(int index)
{
    ui->verticalLayout_5->removeWidget(customPlot);
    customPlot = new QCustomPlot;
    ui->verticalLayout_5->addWidget(customPlot);
    if(index==0){
        QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        customPlot->addPlottable(regen);
        customPlot->addPlottable(nuclear);
        customPlot->addPlottable(fossil);
        // set names and colors:
        QPen pen;
        pen.setWidthF(1.2);
        fossil->setName("空闲");
        pen.setColor(QColor(255, 131, 0));
        fossil->setPen(pen);
        fossil->setBrush(QColor(255, 131, 0, 50));
        nuclear->setName("入住");
        pen.setColor(QColor(1, 92, 191));
        nuclear->setPen(pen);
        nuclear->setBrush(QColor(1, 92, 191, 50));
        regen->setName("损坏");
        pen.setColor(QColor(150, 222, 0));
        regen->setPen(pen);
        regen->setBrush(QColor(150, 222, 0, 70));
        // stack bars ontop of each other:
        nuclear->moveAbove(fossil);
        regen->moveAbove(nuclear);

        // prepare x axis with country labels:
        QVector<double> ticks;
        QVector<QString> labels;
        QVector<double> fossilData, nuclearData, regenData;
        QSqlTableModel * statisticsModel = new QSqlTableModel();
        statisticsModel->setTable("room");
        query->exec("SELECT * FROM roomtype");
        while (query->next()) {
            ticks << query->value("id").toInt();
            labels << query->value("name").toString();
            statisticsModel->setFilter("room_type="+query->value("id").toString()+" AND ( state=0 OR state=2)");
            statisticsModel->select();
            fossilData << statisticsModel->rowCount();
            statisticsModel->setFilter("room_type="+query->value("id").toString()+" AND ( state=1 OR state=3)");
            statisticsModel->select();
            nuclearData << statisticsModel->rowCount();
            statisticsModel->setFilter("room_type="+query->value("id").toString()+" AND state=4");
            statisticsModel->select();
            regenData << statisticsModel->rowCount();
        }
        customPlot->xAxis->setAutoTicks(false);
        customPlot->xAxis->setAutoTickLabels(false);
        customPlot->xAxis->setTickVector(ticks);
        customPlot->xAxis->setTickVectorLabels(labels);
        customPlot->xAxis->setTickLabelRotation(60);
        customPlot->xAxis->setSubTickCount(0);
        customPlot->xAxis->setTickLength(0, 4);
        customPlot->xAxis->grid()->setVisible(true);
        customPlot->xAxis->setRange(0, 8);

        // prepare y axis:
        customPlot->yAxis->setRange(0, 12.1);
        customPlot->yAxis->setPadding(5); // a bit more space to the left border
        customPlot->yAxis->grid()->setSubGridVisible(true);
        QPen gridPen;
        gridPen.setStyle(Qt::SolidLine);
        gridPen.setColor(QColor(0, 0, 0, 25));
        customPlot->yAxis->grid()->setPen(gridPen);
        gridPen.setStyle(Qt::DotLine);
        customPlot->yAxis->grid()->setSubGridPen(gridPen);

        // Add data:
        fossil->setData(ticks, fossilData);
        nuclear->setData(ticks, nuclearData);
        regen->setData(ticks, regenData);

        // setup legend:
        customPlot->legend->setVisible(true);
        customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
        customPlot->legend->setBrush(QColor(255, 255, 255, 200));
        QPen legendPen;
        legendPen.setColor(QColor(130, 130, 130, 200));
        customPlot->legend->setBorderPen(legendPen);
        QFont legendFont = font();
        legendFont.setPointSize(10);
        customPlot->legend->setFont(legendFont);
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }
    else if(index==1){
        QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        QCPBars *fossil = new QCPBars(customPlot->xAxis, customPlot->yAxis);
        customPlot->addPlottable(regen);
        customPlot->addPlottable(nuclear);
        customPlot->addPlottable(fossil);
        // set names and colors:
        QPen pen;
        pen.setWidthF(1.2);
        fossil->setName("空闲");
        pen.setColor(QColor(255, 131, 0));
        fossil->setPen(pen);
        fossil->setBrush(QColor(255, 131, 0, 50));
        nuclear->setName("入住");
        pen.setColor(QColor(1, 92, 191));
        nuclear->setPen(pen);
        nuclear->setBrush(QColor(1, 92, 191, 50));
        regen->setName("损坏");
        pen.setColor(QColor(150, 222, 0));
        regen->setPen(pen);
        regen->setBrush(QColor(150, 222, 0, 70));
        // stack bars ontop of each other:
        nuclear->moveAbove(fossil);
        regen->moveAbove(nuclear);

        // prepare x axis with country labels:
        QVector<double> ticks;
        QVector<QString> labels;
        QVector<double> fossilData, nuclearData, regenData;
        QSqlTableModel * statisticsModel = new QSqlTableModel();
        statisticsModel->setTable("room");
        query->exec("SELECT * FROM roomtype");
        while (query->next()) {
            ticks << query->value("id").toInt();
            labels << query->value("name").toString();
            statisticsModel->setFilter("room_type="+query->value("id").toString()+" AND ( state=0 OR state=1)");
            statisticsModel->select();
            fossilData << statisticsModel->rowCount();
            statisticsModel->setFilter("room_type="+query->value("id").toString()+" AND ( state=2 OR state=3)");
            statisticsModel->select();
            nuclearData << statisticsModel->rowCount();
            statisticsModel->setFilter("room_type="+query->value("id").toString()+" AND state=4");
            statisticsModel->select();
            regenData << statisticsModel->rowCount();
        }
        customPlot->xAxis->setAutoTicks(false);
        customPlot->xAxis->setAutoTickLabels(false);
        customPlot->xAxis->setTickVector(ticks);
        customPlot->xAxis->setTickVectorLabels(labels);
        customPlot->xAxis->setTickLabelRotation(60);
        customPlot->xAxis->setSubTickCount(0);
        customPlot->xAxis->setTickLength(0, 4);
        customPlot->xAxis->grid()->setVisible(true);
        customPlot->xAxis->setRange(0, 8);

        // prepare y axis:
        customPlot->yAxis->setRange(0, 12.1);
        customPlot->yAxis->setPadding(5); // a bit more space to the left border
        customPlot->yAxis->grid()->setSubGridVisible(true);
        QPen gridPen;
        gridPen.setStyle(Qt::SolidLine);
        gridPen.setColor(QColor(0, 0, 0, 25));
        customPlot->yAxis->grid()->setPen(gridPen);
        gridPen.setStyle(Qt::DotLine);
        customPlot->yAxis->grid()->setSubGridPen(gridPen);

        // Add data:
        fossil->setData(ticks, fossilData);
        nuclear->setData(ticks, nuclearData);
        regen->setData(ticks, regenData);

        // setup legend:
        customPlot->legend->setVisible(true);
        customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
        customPlot->legend->setBrush(QColor(255, 255, 255, 200));
        QPen legendPen;
        legendPen.setColor(QColor(130, 130, 130, 200));
        customPlot->legend->setBorderPen(legendPen);
        QFont legendFont = font();
        legendFont.setPointSize(10);
        customPlot->legend->setFont(legendFont);
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }
    else if (index ==2) {
        QVector<double> x,cost,cost1,cost2,costs;
        int i;
        for(i=1 ; (i*24*60)<=nowTime ; ++i){
            x.push_back(i);
            query->exec("SELECT sum(cost) FROM hasleft WHERE end_time <="+QString::number(i*24*60));
            query->next();
            cost.push_back(query->value("sum(cost)").toDouble());
            //qDebug() << query->value("sum(cost)").toDouble();
            query->exec("SELECT sum(cost1) FROM hasleft WHERE end_time <="+QString::number(i*24*60));
            query->next();
            cost1.push_back(query->value("sum(cost1)").toDouble());
            //qDebug() << query->value("sum(cost1)").toDouble();
            query->exec("SELECT sum(cost2) FROM hasleft WHERE end_time <="+QString::number(i*24*60));
            query->next();
            cost2.push_back(query->value("sum(cost2)").toDouble());
            //qDebug() << query->value("sum(cost2)").toDouble();
            costs.push_back(cost.back()+cost1.back()+cost2.back());
        }
        if(i!=1){
            customPlot->addGraph();
            customPlot->graph(0)->setData(x,cost);
            customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCross);
            customPlot->graph(0)->setPen(QPen(Qt::blue));
            customPlot->graph(0)->setName("住宿收入");
            customPlot->addGraph();
            customPlot->graph(1)->setData(x,cost1);
            customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssPlus);
            customPlot->graph(1)->setPen(QPen(Qt::red));
            customPlot->graph(1)->setName("食品收入");
            customPlot->addGraph();
            customPlot->graph(2)->setData(x,cost2);
            customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssCircle);
            customPlot->graph(2)->setPen(QPen(Qt::green));
            customPlot->graph(2)->setName("通话费收入");
            customPlot->addGraph();
            customPlot->graph(3)->setData(x,costs);
            customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssPeace);
            customPlot->graph(3)->setPen(QPen(Qt::yellow));
            customPlot->graph(3)->setName("总收入");
            customPlot->xAxis->setRange(0,i+1);
            customPlot->yAxis->setRange(0,costs.back()+1000);
            customPlot->replot();

            customPlot->legend->setVisible(true);
            customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
            customPlot->legend->setBrush(QColor(255, 255, 255, 200));
            QPen legendPen;
            legendPen.setColor(QColor(130, 130, 130, 200));
            customPlot->legend->setBorderPen(legendPen);
            QFont legendFont = font();
            legendFont.setPointSize(10);
            customPlot->legend->setFont(legendFont);
            customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        }
    }
}
