#-------------------------------------------------
#
# Project created by QtCreator 2017-05-28T15:00:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3dScreenControl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    comport.cpp \
    visualiser.cpp

HEADERS  += mainwindow.h \
    comport.h \
    visualiser.h \
    shared.h

INCLUDEPATH += "/home/konstantin/OpenNI2/Include"
INCLUDEPATH += "/home/konstantin/NiTE/NiTE-Linux-x64-2.2/Include"

LIBS += "/home/konstantin/OpenNI2/Bin/x64-Release/lib*.so"
LIBS += "/home/konstantin/NiTE/NiTE-Linux-x64-2.2/Redist/lib*.so"
