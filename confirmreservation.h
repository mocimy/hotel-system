#ifndef CONFIRMRESERVATION_H
#define CONFIRMRESERVATION_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class ConfirmReservation;
}

class ConfirmReservation : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmReservation(QSqlRecord r,QWidget *parent = 0);
    ~ConfirmReservation();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ConfirmReservation *ui;
    QSqlRecord record;
};

#endif // CONFIRMRESERVATION_H
