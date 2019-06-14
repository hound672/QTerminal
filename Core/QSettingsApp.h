#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#include "App/AppConfig.h"

// ======================================================================

#define SETTING_STR_VAL(GROUP, NAME, DEFAULT) QString NAME;
#define SETTING_INT_VAL(GROUP, NAME, DEFAULT) int NAME;
#define SETTING_BOL_VAL(GROUP, NAME, DEFUALT) bool NAME;

#define SETTINGS_BEGIN_GROUP(GROUP)  \
  struct S##GROUP {

#define SETTINGS_END_GROUP(GROUP)   } GROUP;

// for define settings with existed struct type
#define SETTINGS_TYPED_BEGIN_GROUP(TYPE, GROUP)  \
  TYPE GROUP;

#define SETTINGS_TYPED_END_GROUP(GROUP) 

#define SETTING_TYPED_STR_VAL(GROUP, NAME, DEFAULT)
#define SETTING_TYPED_INT_VAL(GROUP, NAME, DEFAULT)
#define SETTING_TYPED_BOL_VAL(GROUP, NAME, DEFAULT)

// ======================================================================

class QSettingsApp : public QSettings
{
  Q_OBJECT

public:
	MAKE_SETTINGS_TABLE()
	
// ======================================================================

public:
  explicit QSettingsApp(const QString &settingsFileName);
	void SaveSettings();
	
// ======================================================================

private:
  void ReadSettings();
	
// ======================================================================

};

#endif // SETTINGS_H
