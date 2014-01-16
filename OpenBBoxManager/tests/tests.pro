include(../includes.pri)


TEMPLATE = app
TARGET = OpenBBoxManagerTest

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += core
QT       += gui
QT       += network
QT       += testlib

SOURCES +=  main.cpp \
    control/receiverbehaviortcptest.cpp \

HEADERS +=  control/receiverbehaviortcptest.h \


CONFIG   += console
CONFIG   -= app_bundle
CONFIG += testcase

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

LIBS += \
    -lgcov
