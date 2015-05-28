#-------------------------------------------------
#
# Project created by QtCreator 2015-03-11T16:37:55
#
#-------------------------------------------------

QT       += core gui sql

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
    log.cpp

HEADERS  += mainwindow.h \
    chooseroom.h \
    rooms.h

FORMS    += mainwindow.ui \
    chooseroom.ui \
    rooms.ui

DISTFILES +=

RESOURCES += \
    res.qrc \
    qdarkstyle/style.qrc
