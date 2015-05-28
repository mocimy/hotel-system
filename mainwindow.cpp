#include "mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sqlite();   //初始化数据库

    //初始化系统时间
    nowTime=getTime();
    ui->label->setText(timeConvert(nowTime));

    //初始化计时器
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(time()));
    timer->start(100);

    initializeCheckIn();
    initializeLog();
}

MainWindow::~MainWindow()
{
    query->exec("UPDATE info SET value="+QString::number(nowTime)+" WHERE name='time'");
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
            query->exec("UPDATE room SET state=0 WHERE number="+record.value("room").toString());
        }
        if(i>0) QMessageBox::information(this,"自动离店",QString::number(i)+"位客户超时自动离店,详情请查看已离店客户。");
    }
}

//获取键盘事件，使进度条回零
void MainWindow::keyPressEvent(QKeyEvent *){
    ui->progressBar->reset();
}

//获取鼠标点击事件，使进度条回零
void MainWindow::mouseMoveEvent(QMouseEvent *){
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
    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");
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
                        "cost INT,"
                        "cost1 INT,"
                        "cost2 INT,"
                        "cost3 INT)");
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
                             "cost INT,"
                             "cost1 INT,"
                             "cost2 INT,"
                             "cost3 INT)");
                query->exec("CREATE TABLE room ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      "number INT,"
                                      "state INT,"
                                      "room_type INT,"
                                      "floor INT)");
                query->exec("CREATE TABLE log ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "time VARCHAR,"
                            "content VARCHAR)");
                query->exec("CREATE TABLE reservation ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "telphone VARCHAR,"
                            "reservation_time_CN VARCHAR"
                            "time INT,"
                            "room INT)");
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
                query->exec();
                if (!query->execBatch())
                    qDebug() << query->lastError();
                query->exec("CREATE TABLE info ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "name VARCHAR,"
                            "value INT)"
                            );
                query->exec("INSERT INTO info (name,value) VALUES ('time',0)");
                query->exec("INSERT INTO info (name,value) VALUES ('floor',5)");
                query->exec("CREATE TABLE roomtype ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "name VARCHAR,"
                            "price INT)"
                            );
                query->exec("INSERT INTO roomtype (name,price) VALUES ('经济套间',500)");
                query->exec("INSERT INTO roomtype (name,price) VALUES ('普通套间',700)");
                query->exec("INSERT INTO roomtype (name,price) VALUES ('精品套间',900)");
                query->exec("INSERT INTO roomtype (name,price) VALUES ('豪华套间',1200)");
                query->exec("INSERT INTO roomtype (name,price) VALUES ('总统套房',2000)");
            QMessageBox::information(0,"未找到数据库","系统重新初始化!");
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

/*void MainWindow::on_pushButton_clicked()
{
    timer->stop();
    Dialog *w = new Dialog;
    w->exec();
    timer->start(100);
}*/



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
    default:
        break;
    }
}
