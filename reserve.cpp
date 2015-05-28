#include "mainwindow.h"

void MainWindow::initializeReserve(){

}

void MainWindow::on_confirmReservationBtn_clicked()
{
    model->setTable("reservation");
    model->setFilter("time>="+QString::number(nowTime)+"AND time<="+QString::number(nowTime+240));
    model->select();
    ui->reserveView->setModel(model);
    ui->stackedWidget_2->setCurrentIndex(0);
}
void MainWindow::on_newReservationBtn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}

void MainWindow::on_allReservationBtn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}
