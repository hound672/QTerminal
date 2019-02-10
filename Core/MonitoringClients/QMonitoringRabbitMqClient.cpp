#if defined(USE_MONITORING_SERVICE) && defined(USE_MONITORING_RABBITMQ)

#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

#include "QMonitoringRabbitMqClient.h"

// ======================================================================

QMonitoringRabbitMqClient::QMonitoringRabbitMqClient(const QSettingsApp::SRABBIT_MQ &settings, 
																										 const QSettingsApp::SMONITORING_RABBIT_MQ &monSettings) : 
	IMonitoringClient(),
	mIsInit(false),
	mMqSettings(settings),
	mMonSettings(monSettings)
{
}

// ======================================================================

QMonitoringRabbitMqClient::~QMonitoringRabbitMqClient()
{
	delete mExchange;	
}

// ======================================================================

// ======================================================================
//  public                       
// ======================================================================

/**
	* @brief  Проверка на инициализацию
	* @param  
	* @retval 
	*/
bool QMonitoringRabbitMqClient::IsInit()
{
	return mIsInit;
}

// ======================================================================

/**
	* @brief  Инициалзиация клиента
	* @param  
	* @retval 
	*/
int QMonitoringRabbitMqClient::Init()
{
	qMonDebug("Start connect to RabbitMQ");
	mMqClient = new QMqClient(mMqSettings, this);

	connect(mMqClient, &QMqClient::disconnected, this, &QMonitoringRabbitMqClient::SlotMqDisconnected);
	
	// connect to rabbitmq server
	bool res = mMqClient->ConnectToServer();
	if (!res) {
		RETURN_ERROR();
	}
	
	// declare exhange
	mExchange = mMqClient->DeclareExchange(mMonSettings.EXCHANGE);
	if (!mExchange->isDeclared()) {
		RETURN_ERROR();
	}

	mIsInit = true;
	RETURN_OK();
}

// ======================================================================

/**
	* @brief  Отправка данных
	* @param  
	* @retval 
	*/
int QMonitoringRabbitMqClient::Send(const QString &name, const QVariant &val)
{
	// make JSON
	QJsonObject json;
	json.insert(name, QJsonValue::fromVariant(val));	
	mExchange->publish(QJsonDocument(json).toJson(), mMonSettings.QUEUE);	
	return 1;
}

// ======================================================================

// ======================================================================
//  protected slots                       
// ======================================================================

/**
	* @brief  Слот для обработки отключения клиента RabbitMQ
	* @param  
	* @retval 
	*/
void QMonitoringRabbitMqClient::SlotMqDisconnected()
{
	qMonDebug("RabbitMQ is disconnected!");

	QThread::sleep(20);
	qMonDebug("Start...");
	int res = Init();
	qMonDebug("Result reconnect: %d", res);
}

// ======================================================================

#endif // defined(USE_MONITORING_SERVICE) && defined(USE_MONITORING_RABBITMQ)
