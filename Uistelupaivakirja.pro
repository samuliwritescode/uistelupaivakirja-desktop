#-------------------------------------------------
#
# Project created by QtCreator 2010-12-14T19:14:34
#
#-------------------------------------------------

QT       += core gui xml

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
    singletons.cpp \
    dblayer.cpp \
    xmlwriter.cpp \
    controller.cpp \
    method.cpp

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
    singletons.h \
    dblayer.h \
    xmlwriter.h \
    controller.h \
    method.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

macx:ICON = res/icon.icns
win32:RC_FILE = res/icon.rc
