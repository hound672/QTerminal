#include <QDebug>

#include "QSettingsApp.h"

// ======================================================================

QSettingsApp::QSettingsApp(const QString &settingsFileName) :
  QSettings(settingsFileName, QSettings::IniFormat, NULL)
{
  ReadSettings();
}

// ======================================================================

/**
	* @brief  Сохраняет настройки
	* @param  
	* @retval 
	*/
void QSettingsApp::SaveSettings()
{
#ifdef SETTING_STR_VAL
	#undef SETTING_STR_VAL
#endif

#ifdef SETTING_INT_VAL
	#undef SETTING_INT_VAL
#endif

#ifdef SETTING_BOL_VAL
	#undef SETTING_BOL_VAL
#endif

#ifdef SETTINGS_BEGIN_GROUP
	#undef SETTINGS_BEGIN_GROUP
#endif

#ifdef SETTINGS_END_GROUP
	#undef SETTINGS_END_GROUP
#endif
	
#ifdef SETTINGS_TYPED_BEGIN_GROUP
	#undef SETTINGS_TYPED_BEGIN_GROUP
#endif

#ifdef SETTINGS_TYPED_END_GROUP
	#undef SETTINGS_TYPED_END_GROUP
#endif
	
#ifdef SETTING_TYPED_STR_VAL
	#undef SETTING_TYPED_STR_VAL
#endif

#ifdef SETTING_TYPED_INT_VAL
	#undef SETTING_TYPED_INT_VAL
#endif

#ifdef SETTING_TYPED_BOL_VAL
	#undef SETTING_TYPED_BOL_VAL
#endif
	

#define SETTING_STR_VAL(GROUP, NAME, DEFAULT) QSettings::setValue(#NAME, GROUP.NAME);
#define SETTING_INT_VAL(GROUP, NAME, DEFAULT) QSettings::setValue(#NAME, GROUP.NAME);
#define SETTING_BOL_VAL(GROUP, NAME, DEFAULT) QSettings::setValue(#NAME, GROUP.NAME);

#define SETTINGS_BEGIN_GROUP(GROUP)  QSettings::beginGroup(#GROUP);
#define SETTINGS_END_GROUP(GROUP)          QSettings::endGroup();

// ======================================================================
	
#define SETTINGS_TYPED_BEGIN_GROUP(TYPE, GROUP) QSettings::beginGroup(#GROUP);
#define SETTINGS_TYPED_END_GROUP(GROUP) QSettings::endGroup();
#define SETTING_TYPED_STR_VAL(GROUP, NAME, DEFAULT) QSettings::setValue(#NAME, GROUP.NAME);
#define SETTING_TYPED_INT_VAL(GROUP, NAME, DEFAULT) QSettings::setValue(#NAME, GROUP.NAME);
#define SETTING_TYPED_BOL_VAL(GROUP, NAME, DEFAULT) QSettings::setValue(#NAME, GROUP.NAME);
	
  MAKE_SETTINGS_TABLE()
}

// ======================================================================

/**
	* @brief  Считывает настройки
	* @param  
	* @retval 
	*/
void QSettingsApp::ReadSettings()
{
#ifdef SETTING_STR_VAL
	#undef SETTING_STR_VAL
#endif

#ifdef SETTING_INT_VAL
	#undef SETTING_INT_VAL
#endif

#ifdef SETTING_BOL_VAL
	#undef SETTING_BOL_VAL
#endif

#ifdef SETTINGS_BEGIN_GROUP
	#undef SETTINGS_BEGIN_GROUP
#endif

#ifdef SETTINGS_END_GROUP
	#undef SETTINGS_END_GROUP
#endif
	
#ifdef SETTINGS_TYPED_BEGIN_GROUP
	#undef SETTINGS_TYPED_BEGIN_GROUP
#endif

#ifdef SETTINGS_TYPED_END_GROUP
	#undef SETTINGS_TYPED_END_GROUP
#endif
	
#ifdef SETTING_TYPED_STR_VAL
	#undef SETTING_TYPED_STR_VAL
#endif

#ifdef SETTING_TYPED_INT_VAL
	#undef SETTING_TYPED_INT_VAL
#endif

#ifdef SETTING_TYPED_BOL_VAL
	#undef SETTING_TYPED_BOL_VAL
#endif

#define SETTING_STR_VAL(GROUP, NAME, DEFAULT) GROUP.NAME = QSettings::value(#NAME,  \
                                                        DEFAULT).toString();
#define SETTING_INT_VAL(GROUP, NAME, DEFAULT) GROUP.NAME = QSettings::value(#NAME,  \
                                                        DEFAULT).toInt();
#define SETTING_BOL_VAL(GROUP, NAME, DEFAULT) GROUP.NAME = QSettings::value(#NAME,  \
                                                        DEFAULT).toBool();

#define SETTINGS_BEGIN_GROUP(GROUP)  QSettings::beginGroup(#GROUP);
#define SETTINGS_END_GROUP(GROUP)          QSettings::endGroup();

// ======================================================================
	
#define SETTINGS_TYPED_BEGIN_GROUP(TYPE, GROUP) QSettings::beginGroup(#GROUP);
#define SETTINGS_TYPED_END_GROUP(GROUP) QSettings::endGroup();
#define SETTING_TYPED_STR_VAL(GROUP, NAME, DEFAULT) GROUP.NAME = QSettings::value(#NAME,  \
																																DEFAULT).toString();
#define SETTING_TYPED_INT_VAL(GROUP, NAME, DEFAULT) GROUP.NAME = QSettings::value(#NAME,  \
																																DEFAULT).toInt();
#define SETTING_TYPED_BOL_VAL(GROUP, NAME, DEFAULT) GROUP.NAME = QSettings::value(#NAME,  \
																																DEFAULT).toBool();
	
  MAKE_SETTINGS_TABLE()
}

// ======================================================================
