#ifndef ROOMS_H
#define ROOMS_H

#include <QWidget>

namespace Ui {
class Rooms;
}

class Rooms : public QWidget
{
    Q_OBJECT

public:
    explicit Rooms(QWidget *parent = 0,int floor = 0);
    ~Rooms();

private:
    Ui::Rooms *ui;
};

#endif // ROOMS_H
