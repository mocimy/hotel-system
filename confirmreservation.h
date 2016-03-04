#ifndef CONFIRMRESERVATION_H
#define CONFIRMRESERVATION_H

#include <QDialog>

namespace Ui {
class ConfirmReservation;
}

class ConfirmReservation : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmReservation(QWidget *parent = 0);
    ~ConfirmReservation();

private:
    Ui::ConfirmReservation *ui;
};

#endif // CONFIRMRESERVATION_H
