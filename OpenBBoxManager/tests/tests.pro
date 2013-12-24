TEMPLATE = app
INCLUDEPATH += $$TOP_PWD/src/
SRC_DIR = $$TOP_PWD
TARGET = OpenBBoxManagerTest

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += declarative
QT       += core
QT       += gui
QT       += testlib

CONFIG   += console
CONFIG   -= app_bundle
CONFIG += testcase
SOURCES += main.cpp \
    ../src/receiverbehaviortcp.cpp
#        OBBNodeTest.cpp

HEADERS  += ../src/receiverbehaviortcp.h
#        OBBNodeTest.h

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

LIBS += \
    -lgcov

DEFINES += SRCDIR=\\\"$$PWD/\\\"

LIBS += -lunittest++
