#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <QStackedLayout>
#include <QFileInfo>
#include <QMessageBox>
#include <QtSql>
#include <QStackedLayout>
#include <QDataWidgetMapper>
#include "ui_mainwindow.h"
#include "rooms.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //bool LoadData();
    void keyPressEvent(QKeyEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void sqlite();
    int getTime();
    QString timeConvert(const int & nowTime);
    void showError(const QSqlError &err);
    void updateCheckInTable();
    void initializeCheckIn();
    void initializeLog();
    void initializeReserve();
    void setRoom(int roomNum,int price);
    void addLog(QString log);
    ~MainWindow();

private slots:
    void time();
    void on_addItem_clicked();
    void on_swithButton_1_clicked();
    void on_swithButton_2_clicked();
    void on_swithButton_3_clicked();
    void on_btnAddCost_clicked();
    void on_btnLeave_clicked();

    void on_swithButton_4_clicked();

    void on_floorBox_currentIndexChanged(int index);

    void on_chooseRoom_clicked();

    void on_searchEdit_textChanged(const QString &arg1);

    void on_daySpinBox_valueChanged(double arg1);

    void on_searchEdit_2_textChanged(const QString &arg1);

    void on_allReservationBtn_clicked();

    void on_confirmReservationBtn_clicked();

    void on_newReservationBtn_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int nowTime;
    QSqlQuery *query;
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
    Rooms * roomsWidget[99];
    int room_num;
    int cost;
    int end_time;
};

#endif // MAINWINDOW_H
