/**
  * @version  
  * @brief    Клиент для работы с брокером RabbitMQ для класса мониторинга
  */
#ifndef QMONITORINGRABBITMQCLIENT_H
#define QMONITORINGRABBITMQCLIENT_H

#if defined(USE_MONITORING_SERVICE) && defined(USE_MONITORING_RABBITMQ)


#include <QObject>

#include "Core/QMonitoringService.h"
#include "Core/QSettingsApp.h"
#include "Core/Libs/QMqClient.h"

class QMonitoringRabbitMqClient : public IMonitoringClient
{
	Q_OBJECT
	
public:
	explicit QMonitoringRabbitMqClient(const QSettingsApp::SRABBIT_MQ &settings,
																		 const QSettingsApp::SMONITORING_RABBIT_MQ &monitoringSettings);
	~QMonitoringRabbitMqClient();
	// ======================================================================
	bool IsInit();
	int Init();
	int Send(const QString &name, const QVariant &val);

// ======================================================================
	
protected:
	bool mIsInit;
	QMqClient *mMqClient;
	QSettingsApp::SRABBIT_MQ mMqSettings;
	QSettingsApp::SMONITORING_RABBIT_MQ mMonSettings;
	QAmqpExchange *mExchange;
	
// ======================================================================
	
protected slots:
	void SlotMqDisconnected();
};

#endif // defined(USE_MONITORING_SERVICE) && defined(USE_MONITORING_RABBITMQ)

#endif // QMONITORINGRABBITMQCLIENT_H
