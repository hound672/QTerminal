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
	mTimer(NULL),
	mIsInit(false),
	mMqSettings(settings),
	mMonSettings(monSettings)
{
}

// ======================================================================

QMonitoringRabbitMqClient::~QMonitoringRabbitMqClient()
{
	delete mExchange;	
	delete mTimer;
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
	qDebugMon("Start connect to RabbitMQ");
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
	
	if (!mTimer) {
		mTimer = new QTimer(this);
		connect(mTimer,&QTimer::timeout, this, &QMonitoringRabbitMqClient::slotTimeout);
		mTimer->start(mMonSettings.INTERVAL_SEND_DATA);
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
int QMonitoringRabbitMqClient::Send(const QString &name, const QVariant &value)
{
	mMonStore[name] = value;
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
	qDebugMon("RabbitMQ is disconnected!");

	QThread::sleep(20);
	qDebugMon("Start...");
	int res = Init();
	qDebugMon("Result reconnect: %d", res);
}

// ======================================================================

/**
	* @brief  Отправляем данные для сервиса мониторинга
	* @param  
	* @retval 
	*/
void QMonitoringRabbitMqClient::slotTimeout()
{
	// make JSON
	QJsonObject json, monJson;
	
	foreach (QString monName, mMonStore.keys()) {
		monJson.insert(monName, QJsonValue::fromVariant(mMonStore[monName]));
	}
	
	json.insert("service_name", mMonSettings.SERVICE_NAME);
	json.insert("monitoring_data", monJson);
	
	qDebugMon() << "JSON to send: " << json;
	mExchange->publish(QJsonDocument(json).toJson(), mMonSettings.QUEUE);
}

// ======================================================================

#endif // defined(USE_MONITORING_SERVICE) && defined(USE_MONITORING_RABBITMQ)
