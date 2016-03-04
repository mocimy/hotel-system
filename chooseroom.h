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
    explicit chooseRoom(QWidget *parent = 0,int start_time=0,int end_time=0);
    ~chooseRoom();

private slots:
    void on_buttonBox_accepted();

    void on_comboBox_currentIndexChanged(const QString &arg1);

signals:
    void returnInfo(int ,int);

private:
    Ui::chooseRoom *ui;
    int startTime;
    int endTime;
};

#endif // CHOOSEROOM_H
