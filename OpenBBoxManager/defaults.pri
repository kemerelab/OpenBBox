INCLUDEPATH += $$TOP_PWD/src/
SRC_DIR = $$TOP_PWD/src/


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


HEADERS_DIR = $$HEADERS
