TEMPLATE = app
TARGET = robotGUI

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp \
    window.cpp \
    client.cpp \
    car.cpp

HEADERS += \
    window.h \
    client.h \
    car.h
