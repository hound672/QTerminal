QT += core testlib serialport
QT -= gui

include(../../QT_BLANK.pri)
include(../../Core/Core.pri)

CONFIG += c++11

TARGET = TestsCore
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += $$PWD/../../ 

TEMPLATE = app

SOURCES += main.cpp \
    QTestMonitoringService.cpp \
    ../QSetupClass.cpp \
    ../../App/QMainClass.cpp \
    ../QSettingsApp.cpp \
    ../QDebugEx/QDebugEx.cpp \
    QTestMainClass.cpp \
    Libs/QTestComPortThread.cpp \
    ../QThreadWorker.cpp \
    ../Libs/QComPortThread.cpp \
    Utils/QTestStringUtils.cpp \
    QTestBitsSet.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    QTestMonitoringService.h \
    ../QSetupClass.h \
    ../../App/QMainClass.h \
    ../QSettingsApp.h \
    ../QDebugEx/QDebugEx.h \
    QTestMainClass.h \
    ../Includes.h \
    Libs/QTestComPortThread.h \
    ../QThreadWorker.h \
    ../Libs/QComPortThread.h \
    Utils/QTestStringUtils.h \
    QTestBitsSet.h

