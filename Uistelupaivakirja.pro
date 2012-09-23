#-------------------------------------------------
#
# Project created by QtCreator 2010-12-14T19:14:34
#
#-------------------------------------------------

QT       += core gui xml opengl webkit network

TARGET = Uistelupaivakirja
TEMPLATE = app
CONFIG += exceptions


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
    statisticsform.cpp \
    tripstatistics.cpp \
    fishstatistics.cpp \
    glstatwidget.cpp \
    trollingstatisticstable.cpp \
    medialist.cpp \
    messagedisplay.cpp \
    htmlreport.cpp \
    settingsform.cpp \
    routeitem.cpp \
    routeinfo.cpp \
    alternative.cpp \
    serverinterface.cpp \
    simpletrollingobjectfactory.cpp \
    transactionjournal.cpp \
    revisiondialog.cpp \
    synchronizer.cpp \
    synchronizercontroller.cpp \
    reloadingtrollingobjectfactory.cpp

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
    statisticsform.h \
    tripstatistics.h \
    fishstatistics.h \
    glstatwidget.h \
    trollingstatisticstable.h \
    medialist.h \
    messagedisplay.h \
    htmlreport.h \
    settingsform.h \
    routeitem.h \
    routeinfo.h \
    alternative.h \
    serverinterface.h \
    simpletrollingobjectfactory.h \
    transactionjournal.h \
    synchronizer.h \
    revisiondialog.h \
    synchronizercontroller.h \
    reloadingtrollingobjectfactory.h

FORMS    += mainwindow.ui \
    tripform.ui \
    lureform.ui \
    placeform.ui \
    statisticsform.ui \
    settingsform.ui \
    revisiondialog.ui

RESOURCES += \
    resources.qrc

macx:ICON = res/icon.icns
win32:RC_FILE = res/icon.rc
macx:QMAKE_CXXFLAGS += -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk
