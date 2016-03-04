#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T16:37:55
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hotel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    checkin.cpp \
    reserve.cpp \
    setting.cpp \
    chooseroom.cpp \
    rooms.cpp \
    log.cpp \
    confirmreservation.cpp \
    askforpasswd.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    chooseroom.h \
    rooms.h \
    confirmreservation.h \
    askforpasswd.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    chooseroom.ui \
    rooms.ui \
    confirmreservation.ui \
    askforpasswd.ui

DISTFILES +=

RESOURCES += \
    res.qrc \
    qdarkstyle/style.qrc
