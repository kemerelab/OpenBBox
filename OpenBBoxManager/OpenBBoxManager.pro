#-------------------------------------------------
#
# Project created by QtCreator 2013-06-20T14:30:33
#
#-------------------------------------------------

QT       += declarative
QT       += core
QT       += gui
QT       += sql

TARGET = OpenBBoxManager
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        obbnode.cpp \
        receivervideoudp.cpp \
        controller.cpp \
    controlwindow.cpp \
    receiverbehaviortcp.cpp \
    dao/sqlitedatabasemanager.cpp \
    dao/subjectdao.cpp \
    dao/mysqldatabasemanager.cpp \
    dao/openbboxmanagerdao.cpp \
    dao/openbboxnodedao.cpp \
    dao/behaviortaskdao.cpp \
    dao/videostreampacketdao.cpp \
    dao/behavioreventpacketdao.cpp \
    dao/behaviorinfodao.cpp \
    dao/controlcommanddao.cpp \
    dao/taskfiledao.cpp \
    util/logger.cpp

HEADERS  += mainwindow.h \
    config.h \
    controller.h\
    obbnode.h \
    receivervideoudp.h \
    consoleserverpacket.h \
    controlwindow.h \
    streampacket.h \
    receiverbehaviortcp.h \
    dao/sqlitedatabasemanager.h \
    dao/subjectdao.h \
    dao/sqldatabase.h \
    dao/mysqldatabasemanager.h \
    dao/subjectobject.h \
    dao/openbboxmanagerobject.h \
    dao/openbboxmanagerdao.h \
    dao/openbboxnodeobject.h \
    dao/openbboxnodedao.h \
    dao/behaviortaskobject.h \
    dao/behaviortaskdao.h \
    dao/videostreampacketobject.h \
    dao/videostreampacketdao.h \
    dao/behavioreventpacketobject.h \
    dao/behavioreventpacketdao.h \
    dao/behaviorinfodao.h \
    dao/behaviorinfoobject.h \
    dao/controlcommandobject.h \
    dao/controlcommanddao.h \
    dao/taskfileobject.h \
    dao/taskfiledao.h \
    util/logger.h


FORMS    += mainwindow.ui \
    controlwindow.ui


OTHER_FILES +=

RESOURCES += \
    resource.qrc

