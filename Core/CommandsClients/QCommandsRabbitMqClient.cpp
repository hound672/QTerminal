#if defined(USE_COMMANDS_SERVICE) && defined(USE_COMMANDS_RABBITMQ)

#include <QJsonDocument>

#include "QCommandsRabbitMqClient.h"

// ======================================================================
static const QString STR_RESULT = "result";
static const QString STR_COMMAND = "command";
static const QString STR_PARAMS = "params";
// ======================================================================

#undef COMMANDS_TABLE_BEGIN
#undef COMMANDS_TABLE_BODY0	
#undef COMMANDS_TABLE_BODY
#undef COMMANDS_TABLE_END

#define COMMANDS_TABLE_BEGIN				static const QHash<QString, QCommandsService::ECmdTypes> sCmdList = {
#define COMMANDS_TABLE_BODY0(NAME)		{#NAME, QCommandsService::cmd##NAME},		
#define COMMANDS_TABLE_BODY(NAME)			{#NAME, QCommandsService::cmd##NAME},
#define COMMANDS_TABLE_END					};

MAKE_COMMANDS_TABLE()

// ======================================================================

QCommandsRabbitMqClient::QCommandsRabbitMqClient(const QSettingsApp::SRABBIT_MQ &settings, 
																								 const QSettingsApp::SCOMMANDS_RABBIT_MQ &commandsSettings) : 
	ICommandsClient(),
	mMqSettings(settings),
	mCmdSettings(commandsSettings)
{
}

// ======================================================================

// ======================================================================
//  public                        
// ======================================================================

/**
	* @brief  Инициализация клиента 
	* @param  
	* @retval 
	*/
int QCommandsRabbitMqClient::Init()
{
	qCmdDebug("Start connect to RabbitMQ");
	mMqClient = new QMqClient(mMqSettings, this);
	
	connect(mMqClient, &QMqClient::disconnected, this, &QCommandsRabbitMqClient::SlotMqDisconnected);

	// connect to rabbitmq server
	bool res = mMqClient->ConnectToServer();
	if (!res) {
		RETURN_ERROR();
	}
	
	// declare exhange
	mExchange = mMqClient->DeclareExchange(mCmdSettings.EXCHANGE);
	if (!mExchange->isDeclared()) {
		RETURN_ERROR();
	}
	
	QAmqpQueue *queue = mMqClient->DeclareQueue(mCmdSettings.QUEUE, QAmqpQueue::NoOptions);
  if (!queue->isDeclared()) {
    RETURN_ERROR();
  }
	
	if (!mMqClient->Bind(queue, mExchange, mCmdSettings.QUEUE)) {
		RETURN_ERROR();
  }
	
	connect(queue, &QAmqpQueue::messageReceived,
          this, &QCommandsRabbitMqClient::SlotMsgReceive);
	
	if (!queue->consume()) {
    RETURN_ERROR();
  }

	RETURN_OK();
}

// ======================================================================

/**
	* @brief  Отправка ответа
	* @param  
	* @retval 
	*/
void QCommandsRabbitMqClient::SendAnswer(const QCommandsService::SAnswerDesc &answer)
{
	QJsonObject jsonAnswer;
	
	if (answer.mReceiver.count() < recParamCount) {
		qCmdDebug("Error count receiver params");
		return;
	}
	
	switch (answer.mRes)
	{
	case QCommandsService::ansOk:
		jsonAnswer.insert(STR_RESULT, "success");
		jsonAnswer.insert(STR_PARAMS, answer.mParams);
		break;
	// ======================================================================
	case QCommandsService::ansErr:
		jsonAnswer.insert(STR_RESULT, "error");
		jsonAnswer.insert(STR_PARAMS, answer.mParams);
		break;
	// ======================================================================
	case QCommandsService::ansUnknown:
		jsonAnswer.insert(STR_RESULT, "unknown_cmd");
		break;
	// ======================================================================
	case QCommandsService::ansBadParams:
		jsonAnswer.insert(STR_RESULT, "bad_params");
		break;
	// ======================================================================
	default:
		qCmdDebug("Unknown answer type: %d", answer.mRes);
		return;
	}
	
	QString routingKey = answer.mReceiver.at(recParamReplyTo);
	QAmqpMessage::PropertyHash properties;
	properties.insert(QAmqpMessage::CorrelationId, answer.mReceiver.at(recParamCorrId));
	
	mExchange->publish(QJsonDocument(jsonAnswer).toJson(), 
										 routingKey, 
										 properties);
}

// ======================================================================

// ======================================================================
//  protected slots                       
// ======================================================================

/**
	* @brief  Слот обработки входящего сообщения от брокера
	* @param  
	* @retval 
	*/
void QCommandsRabbitMqClient::SlotMsgReceive()
{
	
	QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
  if (!queue) {
    return;
  }

	QAmqpMessage msg = queue->dequeue();	
	queue->ack(msg);
	
	QJsonObject json = QJsonDocument::fromJson(msg.payload()).object();
	QString cmd = json.value(STR_COMMAND).toString();
	QJsonObject params = json.value(STR_PARAMS).toObject();
	
	qCmdDebug() << "Receive msg from RabbitMQ: " << msg.payload()
							<< ". ReplyTo: " << msg.property(QAmqpMessage::ReplyTo).toByteArray()
							<< ". CorrId: " << msg.property(QAmqpMessage::CorrelationId).toByteArray();
	QList<QByteArray> receiverParams;
	receiverParams.append(msg.property(QAmqpMessage::ReplyTo).toByteArray());
	receiverParams.append(msg.property(QAmqpMessage::CorrelationId).toByteArray());
	
	QCommandsService::SCmdDesc cmdDesc(QCommandsService::cmdUnknown, params, receiverParams);
	
	if (!sCmdList.contains(cmd)) {
		QCommandsService::SAnswerDesc answer(cmdDesc);
		answer.mRes = QCommandsService::ansUnknown;
		SendAnswer(answer);
		return;
	}
	
	cmdDesc.mType = sCmdList[cmd];
	emit SignalCommand(cmdDesc);
}

// ======================================================================

/**
	* @brief  Слот для обработки отключения клиента RabbitMQ
	* @param  
	* @retval 
	*/
void QCommandsRabbitMqClient::SlotMqDisconnected()
{
	qCmdDebug("RabbitMQ is disconnected!");

	QThread::sleep(20);
	qCmdDebug("Start...");
	int res = Init();
	qCmdDebug("Result reconnect: %d", res);
}

// ======================================================================

#endif // defined(USE_COMMANDS_SERVICE) && defined(USE_COMMANDS_RABBITMQ)
