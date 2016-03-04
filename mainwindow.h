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
#include "confirmreservation.h"
#include "qcustomplot.h"

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
    void initializeSetting();
    void addLog(QString log);
    void setProgressBar();
    void setApp(QApplication *a);
    void refresh();
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

    void on_doubleSpinBox_valueChanged(double arg1);
    void on_submitBtn_clicked();

    void on_chooseRoom_2_clicked();

    void on_timeEdit_timeChanged(const QTime &time);

    void on_confirmBtn_clicked();

    void on_searchEdit_3_textChanged(const QString &arg1);

    void on_searchEdit_4_textChanged(const QString &arg1);
    void setRoom(int roomNum,int price);
    void acceptConfirm();
    void rejectConfirm();

    void on_submitChangeBtn_clicked();

    void on_delItemBtn_clicked();

    void on_submitChangeBtn_2_clicked();

    void on_delItemBtn_2_clicked();

    void on_toolBox_currentChanged(int index);

    void on_passwordCheckBox_stateChanged(int arg1);

    void on_passwordEdit_editingFinished();

    void on_backupBtn_clicked();

    void on_restoreBtn_clicked();

    void on_delBackupBtn_clicked();
    void on_floorEdit_textChanged(const QString &arg1);

    void on_roomEditBtn_clicked();

    void on_goBackBtn_clicked();

    void on_addRoomTypeBtn_clicked();

    void on_delRoomTypeBtn_clicked();

    void on_addroomBtn_clicked();

    void on_delroomBtn_clicked();

    void on_sortBtn_clicked();

    void on_skinBox_currentIndexChanged(int index);

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_reinitializeBtn_clicked();

    void on_detailBtn_clicked();

    void on_goBackBtn_2_clicked();

    void on_statisticsBox_currentIndexChanged(int index);

    void on_delaySpinBox_valueChanged(double arg1);

    void on_delayBtn_clicked();

    void on_BtnLeave2_clicked();

    void on_favSpinBox_valueChanged(const QString &arg1);

    void on_DataSpinBox_valueChanged(int arg1);

    void on_addManBtn_clicked();

    void on_addWomanBtn_clicked();

private:
    Ui::MainWindow *ui;
    QApplication *app;
    QTimer *timer;
    int nowTime;
    QSqlQuery *query;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlTableModel *roomModel,*roomTypeModel;
    QDataWidgetMapper *mapper;
    Rooms * roomsWidget;
    ConfirmReservation *w ;
    QCustomPlot *customPlot;
    int room_num;
    int cost;
    int start_time;
    int end_time;
    int row ;
    int passwd;
};


#endif // MAINWINDOW_H
