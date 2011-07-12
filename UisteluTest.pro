#-------------------------------------------------
#
# Project created by QtCreator 2011-07-12T15:59:46
#
#-------------------------------------------------
include(Uistelupaivakirja.pro)

QT       +=  testlib

TARGET = UisteluTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += UisteluTest/testserverinterface.cpp \
    UisteluTest/testxmlwriter.cpp \
    UisteluTest/testmain.cpp
SOURCES -= main.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
