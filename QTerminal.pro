# Qt applications and config
QT += core gui widgets serialport
CONFIG += c++11 debug_and_release
CONFIG -= app_bundle
TEMPLATE = app

include(QTerminal.pri)
include(Core/Core.pri)

# defines
DEFINES += DEBUG_USE_UTC
DEFINES += QT_DEPRECATED_WARNINGS

# files in project
SOURCES += \
	App/main.cpp \
    Core/QDebugEx/QDebugEx.cpp \ 
    Core/QSettingsApp.cpp \
    Core/QSetupClass.cpp \
    App/QMainClass.cpp \ 
    Gui/Win/QWinMainWindowTerminal.cpp \
    Core/Libs/QComPortThread.cpp \
    Core/QThreadWorker.cpp \
    Terminal/QTerminal.cpp \
    Core/QStateMachineEx.cpp

HEADERS += \
    Core/QDebugEx/QDebugEx.h \ 
    Core/QSettingsApp.h \
    Core/QSetupClass.h \
    App/QMainClass.h \
    Core/Includes.h \
    App/AppConfig.h \
    Collections/interfaces.h \ 
    Gui/Win/QWinMainWindowTerminal.h \
    Core/Libs/QComPortThread.h \
    Core/QThreadWorker.h \
    Terminal/QTerminal.h \
    Core/QStateMachineEx.h \
    Collections/collections.h

DISTFILES += \
    Core/readme.MD

FORMS += \
    Gui/Win/QWinMainWindowTerminal.ui
