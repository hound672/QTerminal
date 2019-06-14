#include <QThread>
#include <QDateTime>
#include <QDebug>

#include "Core/Includes.h"
#include "QDebugEx/QDebugEx.h"

#include "QSetupClass.h"

#include "Core/MonitoringClients/QMonitoringRabbitMqClient.h"
#include "Core/CommandsClients/QCommandsRabbitMqClient.h"

#ifdef USE_EPOOL_EVENT
	#include "Core/QEventDispatcherEpoll/qeventdispatcher_epoll.h"
#endif // USE_EPOOL_EVENT

// ======================================================================

QSetupClass::QSetupClass(int argc, char *argv[]): 
	QObject(NULL),
	mSettings(SETTINGS_FILE_NAME)
{
#ifdef USE_EPOOL_EVENT
	APP_CLASS::setEventDispatcher(new QEventDispatcherEpoll);
#endif // USE_EPOOL_EVENT
	mApp = new APP_CLASS(argc, argv);

#ifndef Q_OS_WIN
	CatchUnixSignals({SIGINT});
#endif // Q_OS_WIN
	
	connect(mApp, &APP_CLASS::aboutToQuit, this, &QSetupClass::slotQuitApp);
	
	readSettings();
	createLogFile(mSettings.COMMON.LOG_FILE_NAME);
	
	addArgvParams();
	parseArgvParams();
}

// ======================================================================

QSetupClass::~QSetupClass()
{
#ifdef USE_MONITORING_SERVICE 
	mMonitor->StopWorker();
	delete mMonitor;
#endif // USE_MONITORING_SERVICE
	
#ifdef USE_COMMANDS_SERVICE
	mCmdService->StopWorker();
	delete mCmdService;
#endif // USE_COMMANDS_SERVICE
	
	delete mApp;	
}

// ======================================================================

// ======================================================================
//  public                       
// ======================================================================

// ======================================================================

/**
	* @brief  Запуск приложения
	* @param  
	* @retval 
	*/
int QSetupClass::StartApp()
{
	int res;
	
	qInfo() << QString("Start %1. Version: %2").arg(APP_NAME).arg(VERSION);
#ifdef QT_DEBUG
	qInfo() << "Debug build!";
#endif
	
	// инициализация объектов приложения
	
#ifdef USE_MONITORING_SERVICE 
	if (CreateMonitor() < 0) RETURN_ERROR();
#endif // USE_MONITORING_SERVICE
	
#ifdef USE_COMMANDS_SERVICE
	if (CreateCommands() < 0) RETURN_ERROR();
#endif // USE_COMMANDS_SERVICE
	
	res = setup();
	if (res < 0) {
		qCritical("Error setup application: %d", res);
		return -1;
	}
	
	makeSignalSlots();
	
	// запуск объектов приложения
#ifdef USE_MONITORING_SERVICE 
	mMonitor->StartWorker();
#endif // USE_MONITORING_SERVICE
	
#ifdef USE_COMMANDS_SERVICE
	mCmdService->StartWorker();
#endif // USE_COMMANDS_SERVICE
	
	res = start();
	if (res < 0) {
		qCritical("Error start application: %d", res);
		return -1;
	}
	
	return mApp->exec();
}

// ======================================================================

/**
	* @brief  Возвращает имя приложения
	* @param  
	* @retval 
	*/
QString QSetupClass::getAppName() const
{
	return APP_NAME;
}

// ======================================================================

/**
	* @brief  Возвращает версию приложения
	* @param  
	* @retval 
	*/
QString QSetupClass::getAppVersion() const
{
	return VERSION;
}

// ======================================================================

/**
	* @brief  Возвращает имя файла с настройками
	* @param  
	* @retval 
	*/
QString QSetupClass::getSettingsFileName() const
{
	return SETTINGS_FILE_NAME;
}

// ======================================================================

// ======================================================================
//  protected                       
// ======================================================================

#ifndef Q_OS_WIN

/**
	* @brief  Ловим UNIX сигналы
	* @param  
	* @retval 
	*/
void QSetupClass::CatchUnixSignals(std::initializer_list<int> quitSignals)
{
	auto handler = [](int sig) -> void {
          // blocking and not aysnc-signal-safe func are valid
          qDebug() << "Quit the application by signal: " << sig;
          QCoreApplication::quit();
      };

      sigset_t blocking_mask;
      sigemptyset(&blocking_mask);
      for (auto sig : quitSignals)
          sigaddset(&blocking_mask, sig);

      struct sigaction sa;
      sa.sa_handler = handler;
      sa.sa_mask    = blocking_mask;
      sa.sa_flags   = 0;

      for (auto sig : quitSignals)
          sigaction(sig, &sa, nullptr);
	
}

#endif // Q_OS_WIN

// ======================================================================

/**
	* @brief  Создает переменные в окружении в зависимости от переданных параметров
	* @param  
	* @retval 
	*/
void QSetupClass::setEnv(const QStringList vars)
{
	foreach (QString var, vars) {
		QStringList params = var.split('=');
		QString name = params.at(0);
		QString value = params.length() > 1 ? params.at(1) : "1";
		qputenv(name.toLocal8Bit().data(), value.toLocal8Bit());
	}
}

// ======================================================================

/**
	* @brief  Считывает настройки 
	* @param  settingsFileName: ini файл с настройками
	* @retval 
	*/
void QSetupClass::readSettings()
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

#define SETTING_STR_VAL(GROUP, NAME, DEFAULT) << "\n  " << #NAME << ":" << mSettings.GROUP.NAME
#define SETTING_INT_VAL(GROUP, NAME, DEFAULT) << "\n  " << #NAME << ":" << mSettings.GROUP.NAME
#define SETTING_BOL_VAL(GROUP, NAME, DEFAULT) << "\n  " << #NAME << ":" << mSettings.GROUP.NAME
#define SETTINGS_BEGIN_GROUP(GROUP)  << "\n-------------" << #GROUP << "-------------"
#define SETTINGS_END_GROUP(GROUP)
	
// ======================================================================

#define SETTINGS_TYPED_BEGIN_GROUP(TYPE, GROUP) << "\n-------------" << #GROUP << "-------------"
#define SETTINGS_TYPED_END_GROUP(GROUP) 
#define SETTING_TYPED_STR_VAL(GROUP, NAME, DEFAULT) << "\n  " << #NAME << ":" << mSettings.GROUP.NAME
#define SETTING_TYPED_INT_VAL(GROUP, NAME, DEFAULT) << "\n  " << #NAME << ":" << mSettings.GROUP.NAME
#define SETTING_TYPED_BOL_VAL(GROUP, NAME, DEFAULT) << "\n  " << #NAME << ":" << mSettings.GROUP.NAME
	
  qDebug() << "Settings are read:"
  MAKE_SETTINGS_TABLE();
}

// ======================================================================

/**
	* @brief  Создает лог файл
	* @param  
	* @retval 
	*/
void QSetupClass::createLogFile(const QString &logFileName)
{
	QDebugEx::EDebugExFlags flags;
	
#ifdef DEBUG_DEL_OLD
	flags |= QDebugEx::flgDelOld;
#endif
#ifdef DEBUG_ONLY_FILE
	flags |= QDebugEx::flgOnlyFile;
#endif
#ifdef DEBUG_USE_UTC
	flags |= QDebugEx::flgUseUtc;
#endif
	QDebugEx(logFileName, flags);	
}

// ======================================================================

/**
	* @brief  Добавляет стандартный набор параметров командной строки
	* @param  
	* @retval 
	*/
void QSetupClass::addArgvParams()
{
	mArgvParser.addOption({{"e", "env"}, "Set env variables", "env_var"});
}

// ======================================================================

/**
	* @brief  Разбираем данные командные строки
	* @param  
	* @retval 
	*/
void QSetupClass::parseArgvParams()
{
	// разбираем входящие аргументы
	if (!mArgvParser.parse(APP_CLASS::arguments())) {
		qWarning() << "Error parse arguments: " << mArgvParser.errorText();
	}
	if (mArgvParser.isSet("env")) {
		// были переданые значения переменных окружения
		QStringList values = mArgvParser.values("env");
		setEnv(values);
	}	
}

// ======================================================================

#ifdef USE_MONITORING_SERVICE 
/**
	* @brief  Создает сервис мониторинга состояния приложения
	* @param  
	* @retval 
	*/
int QSetupClass::CreateMonitor()
{
	qDebug() << "Create monitoring service";
	
	IMonitoringClient *monClient;
	
#ifdef USE_MONITORING_RABBITMQ
	monClient = new QMonitoringRabbitMqClient(mSettings.RABBIT_MQ, mSettings.MONITORING_RABBIT_MQ);
#endif // USE_MONITORING_RABBITMQ
	
	mMonitor = new QMonitoringService(monClient);
	
	return 1;	
}

#endif // USE_MONITORING_SERVICE

// ======================================================================

#ifdef USE_COMMANDS_SERVICE
/**
	* @brief  Создает сервис обработки команд
	* @param  
	* @retval 
	*/
int QSetupClass::CreateCommands()
{
	qDebug() << "Create commands service";

	ICommandsClient *cmdClient;
	
#ifdef USE_COMMANDS_RABBITMQ
	cmdClient = new QCommandsRabbitMqClient(mSettings.RABBIT_MQ, mSettings.COMMANDS_RABBIT_MQ);
#endif // USE_COMMANDS_RABBITMQ
	
	mCmdService = new QCommandsService(cmdClient);
	
	return 1;
}

#endif // USE_COMMANDS_SERVICE

// ======================================================================
//  public slots                       
// ======================================================================

/**
	* @brief  Слот для обработки ошибок при запуске воркера
	* @param  
	* @retval 
	*/
void QSetupClass::slotWorkerError(int err)
{
	QObject *worker = sender();
	
	qCritical() << "Error worker: " << worker->metaObject()->className()
							<< ". Code: " << err;
	
	mApp->exit(-1);
}

// ======================================================================

// ======================================================================
//  private slots                       
// ======================================================================

/**
	* @brief  Слот обработки завершения работы приложения
	* @param  
	* @retval 
	*/
void QSetupClass::slotQuitApp()
{
	// сохраняем настройки
	mSettings.SaveSettings();
}

// ======================================================================
