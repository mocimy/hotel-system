#include "mainwindow.h"
#include "chooseroom.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication *a;
    a= new QApplication(argc, argv);
    MainWindow w;
    w.setApp(a);
    w.show();

    return a->exec();
}
