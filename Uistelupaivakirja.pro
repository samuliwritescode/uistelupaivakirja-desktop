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
    place.cpp \
    singletons.cpp \
    dblayer.cpp \
    xmlwriter.cpp \
    controller.cpp \
    lurecontroller.cpp \
    gpxreader.cpp \
    locationprovider.cpp \
    waypointlist.cpp \
    waypointitem.cpp \
    trollingobjectfactory.cpp \
    tripform.cpp \
    lureform.cpp \
    placeform.cpp \
    trollingexception.cpp \
    trollingstatistics.cpp \
    statisticscontroller.cpp \
    statisticsform.cpp

HEADERS  += mainwindow.h \
    lureitem.h \
    trollingmodel.h \
    tripcontroller.h \
    placecontroller.h \
    trollingobject.h \
    trip.h \
    fish.h \
    lure.h \
    place.h \
    singletons.h \
    dblayer.h \
    xmlwriter.h \
    controller.h \
    lurecontroller.h \
    gpxreader.h \
    locationprovider.h \
    waypointlist.h \
    waypointitem.h \
    trollingobjectfactory.h \
    tripform.h \
    lureform.h \
    placeform.h \
    trollingexception.h \
    trollingstatistics.h \
    statisticscontroller.h \
    statisticsform.h

FORMS    += mainwindow.ui \
    tripform.ui \
    lureform.ui \
    placeform.ui \
    statisticsform.ui

RESOURCES += \
    resources.qrc

macx:ICON = res/icon.icns
win32:RC_FILE = res/icon.rc
