/**
	* @version  
	* @brief    Файл с индивидуальными настройки для приложения
	*/
#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "Core/AppConfigEx.h"

// ======================================================================
class QMainClass;
extern QMainClass *gMainClass;
// ======================================================================

// набор настроек приложения
#define MAKE_SETTINGS_TABLE()     \
  SETTINGS_BEGIN_GROUP(COMMON)  \
    SETTING_STR_VAL(COMMON, LOG_FILE_NAME, "APP_LOG.log")         \
  SETTINGS_END_GROUP(COMMON)    \
	SETTINGS_BEGIN_GROUP(TERMINAL)  \
    SETTING_BOL_VAL(TERMINAL, AUTO_RECONNECT, false)         \
  SETTINGS_END_GROUP(TERMINAL)    \
	
#endif // APPCONFIG_H
