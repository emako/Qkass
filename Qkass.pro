#-------------------------------------------------
#
# Project created by QtCreator 2017-12-24T15:28:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qkass
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


SOURCES += \
        main.cpp \
        qkass.cpp \
    codeeditor.cpp \
    highlighter.cpp \
    tools/asstt.cpp \
    tools/ass2text.cpp

HEADERS += \
        qkass.h \
    tag.h \
    codeeditor.h \
    highlighter.h \
    tools/asstt.h \
    tools/ass2text.h

FORMS += \
        qkass.ui \
    tools/asstt.ui \
    tools/ass2text.ui

RESOURCES += \
    res.qrc

RC_FILE = main.rc

DISTFILES +=
