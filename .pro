QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snake_game
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

RESOURCES += photo_bg.qrc  
