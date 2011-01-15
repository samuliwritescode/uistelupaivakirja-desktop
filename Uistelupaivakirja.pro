#-------------------------------------------------
#
# Project created by QtCreator 2010-12-14T19:14:34
#
#-------------------------------------------------

QT       += core gui

TARGET = Uistelupaivakirja
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lureitem.cpp \
    trollingmodel.cpp \
    tripcontroller.cpp \
    placecontroller.cpp \
    trollingobject.cpp \
    trip.cpp \
    fish.cpp \
    lure.cpp \
    site.cpp \
    species.cpp \
    singletons.cpp

HEADERS  += mainwindow.h \
    lureitem.h \
    trollingmodel.h \
    tripcontroller.h \
    placecontroller.h \
    trollingobject.h \
    trip.h \
    fish.h \
    lure.h \
    site.h \
    species.h \
    singletons.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
