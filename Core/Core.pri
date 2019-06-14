# define application version
VERSION_DEFINE=\\\"$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}.$${VERSION_REVISION}\\\"
DEFINES += VERSION=$${VERSION_DEFINE}
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}.$${VERSION_REVISION}

# define application name
DEFINES += APP_NAME=\\\"$${APP_NAME}\\\"

#define application settings file name
DEFINES += SETTINGS_FILE_NAME=\\\"$${SETTINGS_FILE_NAME}\\\"

# if debug we'll delete old log file
Debug:DEFINES+=DEBUG_DEL_OLD

# define target name
TARGET = $${APP_NAME}

TEMPLATE = app

HEADERS += \
    $$PWD/Utils/QStringUtils.h \
    $$PWD/Utils/QBitsSet.h

SOURCES += \
    $$PWD/Utils/QStringUtils.cpp \
    $$PWD/Utils/QBitsSet.cpp
