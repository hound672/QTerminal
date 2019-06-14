/**
  * @version  
  * @brief    Класс-мониторинг состояния приложения
  */
#ifndef QMONITORINGSERVICE_H
#define QMONITORINGSERVICE_H

#ifdef USE_MONITORING_SERVICE 

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QThread>

#include "Core/QThreadWorker.h"

#include "App/AppConfig.h"

// ======================================================================
#define qDebugMon if (!IS_ENV_SET("QMONITORSERVICE_DEBUG")); else qInfo
// ======================================================================

// ======================================================================
// интерфейс описывающий работу для экспорта данных
class IMonitoringClient : public QObject
{
	Q_OBJECT
public:
	explicit IMonitoringClient() : QObject(NULL) {}
	virtual ~IMonitoringClient() {}
	// инициализация клиента
	virtual bool IsInit() = 0;
	virtual int Init() = 0;
	virtual int Send(const QString &name, const QVariant &value) = 0;
};

// ======================================================================

#define MONITORING_TABLE_BEGIN					enum EMonTypes {
#define MONITORING_TABLE_BODY0(NAME)			mon##NAME = 0,				
#define MONITORING_TABLE_BODY(NAME)				mon##NAME,
#define MONITORING_TABLE_END						};

class QMonitoringService : public QThreadWorker
{
	Q_OBJECT
	
public:
	MAKE_MONITORING_TABLE()
	
// ======================================================================
	
public:
	explicit QMonitoringService(IMonitoringClient *client);
	~QMonitoringService();

// ======================================================================	

protected slots:
	void slotStarted();
	void slotStopped();
	
// ======================================================================

protected:
	IMonitoringClient *mClient;	
	
// ======================================================================
	
public slots:
	void slotMonitoring(QMonitoringService::EMonTypes monType, const QVariant &monValue);
	
};


#endif // USE_MONITORING_SERVICE

#endif // QMONITORINGSERVICE_H
