#ifndef CHOOSEROOM_H
#define CHOOSEROOM_H

#include <QDialog>

namespace Ui {
class chooseRoom;
}

class chooseRoom : public QDialog
{
    Q_OBJECT

public:
    explicit chooseRoom(QWidget *parent = 0);
    ~chooseRoom();

private slots:
    void on_buttonBox_accepted();

signals:
    void returnInfo(int ,int);

private:
    Ui::chooseRoom *ui;
};

#endif // CHOOSEROOM_H
