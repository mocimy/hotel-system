#include "confirmreservation.h"
#include "ui_confirmreservation.h"

ConfirmReservation::ConfirmReservation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmReservation)
{
    ui->setupUi(this);
}

ConfirmReservation::~ConfirmReservation()
{
    delete ui;
}
