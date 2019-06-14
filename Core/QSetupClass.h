#ifndef QSETUPCLASS_H
#define QSETUPCLASS_H

#include <QObject>
#include <QCommandLineParser>
#include <QCommandLineOption>

#ifndef Q_OS_WIN
	#include <initializer_list>
	#include <signal.h>
	#include <unistd.h>
#endif // Q_OS_WIN

#include "Core/QMonitoringService.h"
#include "Core/QCommandsService.h"
#include "Core/Includes.h"

#include "QSettingsApp.h"

// ======================================================================

#ifdef QT_GUI_LIB
	#include <QApplication>
	#define APP_CLASS QApplication
#else
	#include <QCoreApplication>
	#define APP_CLASS QCoreApplication
#endif

// макрос возвращающий объект QMainClass
#define MAIN_CLASS() QMainClass::getMainClass()
// макрос возвращающий объект с настройками приложения
#define APP_SETTINGS() QMainClass::getMainClass()->getSettings()

// ======================================================================

class QSetupClass : public QObject
{
	Q_OBJECT
	
public:
	explicit QSetupClass(int argc, char *argv[]);
	virtual ~QSetupClass();
	int StartApp();
	// ======================================================================
	QSettingsApp *getSettings() {return &mSettings;}
	QString getAppName() const;
	QString getAppVersion() const;
	QString getSettingsFileName() const;
#ifdef USE_MONITORING_SERVICE
	QMonitoringService *GetMonitorService()
	{
		return mMonitor;
	}
#endif // USE_MONITORING_SERVICE

#ifdef USE_COMMANDS_SERVICE
	QCommandsService *GetCommandsService()
	{
		return mCmdService;
	}

#endif // USE_COMMANDS_SERVICE
	
	
// ======================================================================
	
protected:
#ifndef Q_OS_WIN
	void CatchUnixSignals(std::initializer_list<int> quitSignals);
#endif // Q_OS_WIN
	void setEnv(const QStringList vars);
	void readSettings();
	void createLogFile(const QString &logFileName);
	void addArgvParams();
	void parseArgvParams();
	virtual void makeSignalSlots() = 0;
	virtual int setup() = 0;
	virtual int start() = 0;
	// ======================================================================
#ifdef USE_MONITORING_SERVICE 
	int CreateMonitor();
	QMonitoringService *mMonitor;
#endif // USE_MONITORING_SERVICE
#ifdef USE_COMMANDS_SERVICE
	int CreateCommands();
	QCommandsService *mCmdService;
#endif // USE_COMMANDS_SERVICE
	
// ======================================================================

protected:
	APP_CLASS *mApp;
	QSettingsApp mSettings;
	QCommandLineParser mArgvParser;
	
// ======================================================================
		
public slots:
	void slotWorkerError(int err);
	
// ======================================================================
	
private slots:
	void slotQuitApp();
};

#endif // QSETUPCLASS_H
