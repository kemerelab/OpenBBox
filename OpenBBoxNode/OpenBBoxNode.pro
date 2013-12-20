#-------------------------------------------------
#
# Project created by QtCreator 2013-06-24T20:13:00
#
#-------------------------------------------------

QT       += core
QT       += script
QT       -= gui

TARGET = OpenBBoxNode
    target.files = OpenBBoxNode
    target.path = /home/root
INSTALLS += target

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    controller.cpp \
    camerasender.cpp \
    udpsender.cpp \
    behaviorcontextsender.cpp \
    tcpsender.cpp \
    MedPCInterpret/statemachine.cpp \
    MedPCInterpret/medpcinterpret.cpp \
    MedPCInterpret/event.cpp \
    MedPCInterpret/context.cpp \
    gpio.cpp \
    util/logger.cpp

HEADERS += \
    controller.h \
    camerasender.h \
    udpsender.h \
    behaviorcontextsender.h \
    tcpsender.h \
    MedPCInterpret/statemachine.h \
    MedPCInterpret/medpcinterpret.h \
    MedPCInterpret/event.h \
    MedPCInterpret/context.h \
    gpio.h \
    util/logger.h

#include config
HEADERS += ../OpenBBoxManager/config.h
