#-------------------------------------------------
#
# Project created by QtCreator 2021-05-11T15:26:01
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FSAqua
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        fsaquawindow.cpp \
    formtools.cpp \
    gbadobject.cpp \
    gpipe.cpp \
    scene.cpp \
    formformularvs.cpp \
    gbadobjectinfodialog.cpp \
    gpipeinfodialog.cpp \
    acceptwidget.cpp \
    pipelistwidget.cpp \
    simulationsllider.cpp \
    en806.cpp \
    garmatur.cpp \
    garmaturinfodialog.cpp

HEADERS += \
        fsaquawindow.h \
    formtools.h \
    gbadobject.h \
    gpipe.h \
    scene.h \
    formformularvs.h \
    gbadobjectinfodialog.h \
    gpipeinfodialog.h \
    acceptwidget.h \
    pipelistwidget.h \
    simulationsllider.h \
    en806.h \
    garmatur.h \
    garmaturinfodialog.h

FORMS += \
        fsaquawindow.ui \
    formtools.ui \
    formformularvs.ui \
    gbadobjectinfodialog.ui \
    gpipeinfodialog.ui \
    acceptwidget.ui \
    pipelistwidget.ui \
    simulationsllider.ui \
    garmaturinfodialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
