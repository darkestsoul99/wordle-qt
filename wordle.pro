#-------------------------------------------------
#
# Project created by QtCreator 2024-01-09T10:51:13
#
#-------------------------------------------------

QT       += core gui
QT       += quickwidgets
QT       += quick
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wordle
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    resultsdialog.cpp \
    settings.cpp \
    howtoplay.cpp

HEADERS += \
        mainwindow.h \
    resultsdialog.h \
    settings.h \
    howtoplay.h

FORMS += \
        mainwindow.ui \
    resultsdialog.ui \
    settings.ui \
    howtoplay.ui

RESOURCES += \
    src.qrc
