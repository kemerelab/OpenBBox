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
    util/logger.cpp \
    util/serializablepacket.cpp \
    util/serializableclass.cpp

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
    util/logger.h \
    util/serializablepacket.h \
    util/serializableclass.h

FORMS    += mainwindow.ui \
    controlwindow.ui


# ##############################################################################
# ##############################################################################
# FFMPEG: START OF CONFIGURATION BELOW ->
# Copy these lines into your own project
# Make sure to set the path variables for:
# 1) QTFFmpegWrapper sources (i.e. where the QVideoEncoder.cpp and QVideoDecoder.cpp lie),
# 2) FFMPEG include path (i.e. where the directories libavcodec, libavutil, etc. lie),
# 3) the binary FFMPEG libraries (that must be compiled separately).
# Under Linux path 2 and 3 may not need to be set as these are usually in the standard include and lib path.
# Under Windows, path 2 and 3 must be set to the location where you placed the FFMPEG includes and compiled binaries
# Note that the FFMPEG dynamic librairies (i.e. the .dll files) must be in the PATH
# ##############################################################################
# ##############################################################################
# ##############################################################################
# Modify here: set FFMPEG_LIBRARY_PATH and FFMPEG_INCLUDE_PATH
# ##############################################################################
# Set QTFFMPEGWRAPPER_SOURCE_PATH to point to the directory containing the QTFFmpegWrapper sources
QTFFMPEGWRAPPER_SOURCE_PATH = ./QTFFmpegWrapper

# Set FFMPEG_LIBRARY_PATH to point to the directory containing the FFmpeg import libraries (if needed - typically for Windows), i.e. the dll.a files
FFMPEG_LIBRARY_PATH = ./ffmpeg_lib_win32

# Set FFMPEG_INCLUDE_PATH to point to the directory containing the FFMPEG includes (if needed - typically for Windows)
FFMPEG_INCLUDE_PATH = ./QTFFmpegWrapper

# ##############################################################################
# Do not modify: FFMPEG default settings
# ##############################################################################
# Sources for QT wrapper
SOURCES += $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoEncoder.cpp \
         $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoDecoder.cpp
HEADERS += $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoEncoder.h \
         $$QTFFMPEGWRAPPER_SOURCE_PATH/QVideoDecoder.h

# Set list of required FFmpeg libraries
LIBS += -lavutil \
         -lavcodec \
         -lavformat \
         -lswscale

# Add the path
LIBS += -L$$FFMPEG_LIBRARY_PATH
INCLUDEPATH += QVideoEncoder
INCLUDEPATH += $$FFMPEG_INCLUDE_PATH

# Requied for some C99 defines
DEFINES += __STDC_CONSTANT_MACROS
# ##############################################################################
# FFMPEG: END OF CONFIGURATION
# ##############################################################################

OTHER_FILES +=

RESOURCES += \
    resource.qrc

