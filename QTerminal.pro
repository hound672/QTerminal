# Qt applications and config
QT += core gui widgets
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

HEADERS += \
    Core/QDebugEx/QDebugEx.h \ 
    Core/QSettingsApp.h \
    Core/QSetupClass.h \
    App/QMainClass.h \
    Core/Includes.h \
    App/AppConfig.h \

DISTFILES += \
    Core/readme.MD
