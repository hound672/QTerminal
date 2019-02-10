#ifndef QCOMMANDSRABBITMQCLIENT_H
#define QCOMMANDSRABBITMQCLIENT_H

#if defined(USE_COMMANDS_SERVICE) && defined(USE_COMMANDS_RABBITMQ)

#include <QObject>

#include "Core/QCommandsService.h"
#include "Core/Libs/QMqClient.h"

class QCommandsRabbitMqClient : public ICommandsClient
{
	Q_OBJECT
	
public:
	explicit QCommandsRabbitMqClient(const QSettingsApp::SRABBIT_MQ &settings,
																	 const QSettingsApp::SCOMMANDS_RABBIT_MQ &commandsSettings);
	// ======================================================================
	int Init();
	void SendAnswer(const QCommandsService::SAnswerDesc &answer);
	
// ======================================================================
	
protected:
	enum EReceiverParams {
		recParamReplyTo = 0,
		recParamCorrId,
		recParamCount
	};
	
// ======================================================================
	
protected:
	QMqClient *mMqClient;
	QSettingsApp::SRABBIT_MQ mMqSettings;
	QSettingsApp::SCOMMANDS_RABBIT_MQ mCmdSettings;
	QAmqpExchange *mExchange;
	
// ======================================================================
	
protected slots:
	void SlotMsgReceive();
	void SlotMqDisconnected();
	
};

#endif // defined(USE_COMMANDS_SERVICE) && defined(USE_COMMANDS_RABBITMQ)

#endif // QCOMMANDSRABBITMQCLIENT_H
