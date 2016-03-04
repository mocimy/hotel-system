#include "mainwindow.h"
#include "dialog.h"
#include "askforpasswd.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sqlite();   //初始化数据库
    //初始化系统时间
    nowTime=getTime();
    ui->label->setText(timeConvert(nowTime));
    passwd=1;
    //初始化计时器
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time()));
    timer->start(100);

    initializeCheckIn();
    initializeSetting();

}

MainWindow::~MainWindow()
{
    query->exec("UPDATE info SET value="+QString::number(nowTime)+" WHERE name='time'");
    //qDebug() << query->lastError();
    db.close();
    delete ui;
}

//获得时间事件，使进度条滚动
void MainWindow::time(){
    ui->progressBar->setValue((ui->progressBar->value())+1);
    if(ui->progressBar->value()==100){
        nowTime+=20;
        ui->label->setText(timeConvert(nowTime));
        ui->progressBar->reset();

        QSqlTableModel * leaveModel = new QSqlTableModel();
        QSqlTableModel * hasLeftModel = new QSqlTableModel();
        leaveModel->setTable("hotel");
        hasLeftModel->setTable("hasleft");
        leaveModel->setFilter("end_time <="+QString::number(nowTime));
        leaveModel->select();
        int i ;
        for( i = 0 ; i < leaveModel->rowCount() ; ++i ){
            QSqlRecord record = leaveModel->record(i);
            leaveModel->removeRow(i);
            hasLeftModel->insertRecord(0,record);
            query->exec("UPDATE room SET state=0 WHERE state=1 AND number="+record.value("room").toString());
            query->exec("UPDATE room SET state=2 WHERE state=3 AND number="+record.value("room").toString());
        }
        if(i>0) QMessageBox::information(this,"自动离店",QString::number(i)+"位客户超时自动离店,详情请查看已离店客户。");
        leaveModel->setTable("reservation");
        leaveModel->setFilter("start_time<="+QString::number(nowTime-4*60));
        leaveModel->select();
        for( i = 0 ; i < leaveModel->rowCount() ; ++i ) {
            QSqlTableModel *hasReservationModel = new QSqlTableModel;
            hasReservationModel->setTable("reservation");
            hasReservationModel->setFilter("room="+model->record(row).value("room").toString());
            hasReservationModel->select();
            if(hasReservationModel->rowCount()==1) {
                query->exec("UPDATE room SET state=1 WHERE state=3 AND number="+leaveModel->record(i).value("room").toString());
                query->exec("UPDATE room SET state=0 WHERE state=2 AND number="+leaveModel->record(i).value("room").toString());
            }
            leaveModel->removeRow(i);
        }
        if(i>0) QMessageBox::information(this,"自动取消预约",QString::number(i)+"位客户超时自动取消预约。");
        if(ui->tabWidget->currentIndex()==2&&ui->toolBox->currentIndex()==0) on_toolBox_currentChanged(0);
        else if (ui->tabWidget->currentIndex()==3) refresh();
        else model->select();
    }
}

void MainWindow::setApp(QApplication *a){
    app=a;
}

//获取键盘事件，使进度条回零
void MainWindow::keyPressEvent(QKeyEvent *){
    ui->progressBar->reset();
}

//获取鼠标点击事件，使进度条回零
void MainWindow::mouseMoveEvent(QMouseEvent *){
    ui->progressBar->reset();
}

void MainWindow::setProgressBar(){
    ui->progressBar->reset();
}

//获取鼠标移动事件，使进度条回零
void MainWindow::mousePressEvent(QMouseEvent *){
    ui->progressBar->reset();
}

//初始化数据库
void MainWindow::sqlite()
{
    QFileInfo file("data.db");
    bool dbExist=file.exists(); //检查数据库是否存在
    db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");
    if(!db.open()) {
        showError(db.lastError());
        return ;
    }
    query= new QSqlQuery;
    model = new QSqlTableModel(this,db);
    if(!dbExist){   //创建新数据库

        query->exec("CREATE TABLE hotel ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "name VARCHAR,"
                       "sex VARCHAR,"
                       "age INT,"
                       "ID_number INT,"
                       "workplace VARCHAR,"
                       "city VARCHAR,"
                       "room INT,"
                        "start_time_CN VARCHAR,"
                        "end_time INT,"
                        "end_time_CN VARCHAR,"
                        "cost REAL,"
                        "cost1 REAL,"
                        "cost2 REAL,"
                        "cost3 REAL,"
                        "deposit REAL,"
                        "total_deposit REAL)");
                query->exec("CREATE TABLE hasLeft ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "name VARCHAR,"
                            "sex VARCHAR,"
                            "age INT,"
                            "ID_number INT,"
                            "workplace VARCHAR,"
                            "city VARCHAR,"
                            "room INT,"
                             "start_time_CN VARCHAR,"
                             "end_time INT,"
                             "end_time_CN VARCHAR,"
                             "cost REAL,"
                             "cost1 REAL,"
                             "cost2 REAL,"
                             "cost3 REAL,"
                             "deposit REAL,"
                             "total_deposit REAL)");
                query->exec("CREATE TABLE room ("
                                      "number INT,"
                                      "state INT,"
                                      "room_type INT,"
                                      "floor INT)");

                query->prepare("INSERT INTO room (number,state,room_type,floor)" "VALUES (?,?,?,?)");
                QVariantList numbers,states,roomTypes,floors;
                for ( int i = 0 ; i<5 ; ++i)
                    for (int j = 0 ; j<(20-i*3) ; ++j ){
                        numbers << ((i+1)*100+j);
                        states << 0;
                        roomTypes << (i+1);
                        floors << (i+1);
                    }
                query->addBindValue(numbers);
                query->addBindValue(states);
                query->addBindValue(roomTypes);
                query->addBindValue(floors);
                query->execBatch();

                query->exec("CREATE TABLE log ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "time VARCHAR,"
                            "content VARCHAR)");
                query->exec("CREATE TABLE reservation ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "name VARCHAR,"
                            "telphone VARCHAR,"
                            "reservation_time_CN VARCHAR,"
                            "start_time INT,"
                            "start_time_CN VARCHAR,"
                            "end_time INT,"
                            "end_time_CN VARCHAR,"
                            "room INT)");
                qDebug() << query->lastError();
                query->exec("CREATE TABLE info ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "name VARCHAR,"
                            "value INT,"
                            "value_text VARCHAR)"
                            );
                query->exec("INSERT INTO info (name,value) VALUES ('time',0)");
                query->exec("INSERT INTO info (name,value) VALUES ('floor',5)");
                query->exec("INSERT INTO info (name,value) VALUES ('password',0)");
                query->exec("INSERT INTO info (name,value) VALUES ('preferential',8)");
                query->exec("CREATE TABLE roomtype ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "name VARCHAR,"
                            "price REAL)"
                            );
                query->exec("INSERT INTO roomtype (name,price) VALUES ('经济套间',500)");
                query->exec("INSERT INTO roomtype (name,price) VALUES ('普通套间',700)");
                query->exec("INSERT INTO roomtype (name,price) VALUES ('精品套间',900)");
                query->exec("INSERT INTO roomtype (name,price) VALUES ('豪华套间',1200)");
                query->exec("INSERT INTO roomtype (name,price) VALUES ('总统套房',2000)");
            QMessageBox::information(0,"未找到数据库","系统重新初始化!");
            addLog("未找到数据库 ,系统重新初始化");
    }
}

int MainWindow::getTime(){
    query->exec("SELECT value FROM info WHERE name='time'");
    query->first();
    return query->value("value").toInt();
}

QString MainWindow::timeConvert(const int & nowTime){
    QString day,hour,minute;
    day.setNum(nowTime/(24*60));
    hour.setNum((nowTime/60)%24);
    minute.setNum(nowTime%60);
    return "第"+day+"天"+ hour+"时"+ minute+"分";
}

void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(0, "Unable to initialize Database",
                "Error initializing database: " + err.text());
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 0:
        updateCheckInTable();
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case 1:
        initializeReserve();
        break;
    case 2:
        initializeLog();
        break;
    case 3:
        query->exec("SELECT value FROM info WHERE name='password'");
        query->next();
        if(query->value("value").toInt()==1&&passwd==1){
            AskForPasswd w;
            if(w.exec()!=AskForPasswd::Accepted) ui->tabWidget->setCurrentIndex(0);
            else passwd=0;
        }
        else refresh();
        break;
    default:
        break;
    }
}


