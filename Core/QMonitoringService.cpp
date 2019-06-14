#ifdef USE_MONITORING_SERVICE 

#include <QDebug>
#include <QThread>
#include <QDateTime>

#include "Core/Includes.h"

#include "QMonitoringService.h"

// ======================================================================

#undef MONITORING_TABLE_BEGIN
#undef MONITORING_TABLE_BODY0
#undef MONITORING_TABLE_BODY
#undef MONITORING_TABLE_END

#define MONITORING_TABLE_BEGIN				static const QHash<QMonitoringService::EMonTypes, QString> sMonNames = {
#define MONITORING_TABLE_BODY0(NAME)		{QMonitoringService::mon##NAME, #NAME},	
#define MONITORING_TABLE_BODY(NAME)			{QMonitoringService::mon##NAME, #NAME},
#define MONITORING_TABLE_END					};

MAKE_MONITORING_TABLE()

// ======================================================================


QMonitoringService::QMonitoringService(IMonitoringClient *client) : 
	QThreadWorker(),
	mClient(client)
{
	qRegisterMetaType<QMonitoringService::EMonTypes>("QMonitoringService::EMonTypes");
	mClient->moveToThread(mThread);
}

// ======================================================================

QMonitoringService::~QMonitoringService()
{
	delete mClient;
}

// ======================================================================

// ======================================================================
//  protected slots                       
// ======================================================================

/**
	* @brief  Запуск воркера
	* @param  
	* @retval 
	*/
void QMonitoringService::slotStarted()
{
	int res;
		
	res = mClient->Init();
	if (res < 0) {
		qCritical("Error init Monitoring Client: %d", res);
		WORKER_ERROR();
	}
	
	qInfo("Monitoring service is started");
	slotMonitoring(QMonitoringService::monAppStart, QDateTime::currentMSecsSinceEpoch());
}

// ======================================================================

/**
	* @brief  Остановка воркера
	* @param  
	* @retval 
	*/
void QMonitoringService::slotStopped()
{
	qInfo("Monitoring service is stopped");
}

// ======================================================================

// ======================================================================
//  public slots                      
// ======================================================================

/**
	* @brief  Слот обработки состояния приложения
	* @param  
	* @retval 
	*/
void QMonitoringService::slotMonitoring(EMonTypes monType, const QVariant &monValue)
{ 
	if (!mClient->IsInit()) return;
	QString monName = sMonNames[monType];
	mClient->Send(monName, monValue);
}

// ======================================================================

#endif // USE_MONITORING_SERVICE
