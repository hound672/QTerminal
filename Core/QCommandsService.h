/**
	* @version  
	* @brief    Класс для работы с командами
	* 
	*						Создается объект наследник ICommandsClient.
	*						В котором обеспечивается получение данных для команды и отправка данных для ответа.
	*						Этот объект передается в конструктор воркера.
	*						В классе, который будет получать команду должен быть определен метод-слот:
	*						void SlotCommandGot(const QCommandsService::SCmdDesc &cmd);
	*						а в конструкторе вызываться макрос SUBSCRIBE_COMMANDS
	*/
#ifndef QCOMMANDSSERVICE_H
#define QCOMMANDSSERVICE_H

#ifdef USE_COMMANDS_SERVICE

#include <QObject>
#include <QJsonObject>

#include "Core/QThreadWorker.h"
#include "Core/QMonitoringService.h"
#include "Core/Includes.h"

#include "App/AppConfig.h"

// ======================================================================
#define qCmdDebug if (!IS_ENV_SET("QCOMMANDSSERVICE_DEBUG")); else qInfo
// ======================================================================

// ======================================================================
class ICommandsClient;
// ======================================================================

#define COMMANDS_TABLE_BEGIN					enum ECmdTypes {
#define COMMANDS_TABLE_BODY0(NAME)			cmd##NAME = 0,				
#define COMMANDS_TABLE_BODY(NAME)				cmd##NAME,
#define COMMANDS_TABLE_END						cmdCount, cmdUnknown = cmdCount};

class QCommandsService : public QThreadWorker
{
	Q_OBJECT
																	
public:
	MAKE_COMMANDS_TABLE()	
	
	enum EAnswerRes {
		ansOk = 0,
		ansErr,
		ansUnknown,
		ansBadParams
	};
																																		
// ======================================================================
																	
public:
		
	// структура описывающая входящую команду
	struct SCmdDesc {
		QCommandsService::ECmdTypes mType;
		QJsonObject mParams;
		QList<QByteArray> mReceiver;	// параметры получателя
		// ======================================================================
		SCmdDesc() {}
		SCmdDesc(ECmdTypes type, const QJsonObject &params, const QList<QByteArray> &receiver):
			mType(type), mParams(params), mReceiver(receiver) {}
	};
	// ======================================================================
	// структура описывающая ответ на команду
	struct SAnswerDesc {
		ECmdTypes mCmd;								// тип команды, на которую был ответ
		EAnswerRes mRes;							// результат выполнения команды
		QJsonValue mParams;					// параметры ответа
		QList<QByteArray> mReceiver;	// параметры получателя
		// ======================================================================
		SAnswerDesc() {}
		SAnswerDesc(const SCmdDesc &cmd):
			mCmd(cmd.mType), mReceiver(cmd.mReceiver) {}
	};
																	
// ======================================================================
	
public:
	explicit QCommandsService(ICommandsClient *client);
	~QCommandsService();

// ======================================================================
	
protected slots:
	void slotStarted();
	void slotStopped();
	void SlotGotCommandFromClient(const QCommandsService::SCmdDesc &cmd);
	
// ======================================================================
	
public slots:
	void slotSendAnswer(const QCommandsService::SAnswerDesc &answer);
	
// ======================================================================
	
protected:
	ICommandsClient *mClient;
	
// ======================================================================
	
signals:
	void SignalGotCommand(const SCmdDesc &cmd);	// сигнал о получении команды для приложения
	
};
	
// ======================================================================

// ======================================================================
//  ICommandsClient                        
//  интерфейс описывающий работу клиента для команд
// ======================================================================
class ICommandsClient : public QObject
{
	Q_OBJECT
public:
	explicit ICommandsClient() : QObject(NULL) {}
	virtual ~ICommandsClient() {}
	// ======================================================================
	virtual int Init() = 0;
	virtual void SendAnswer(const QCommandsService::SAnswerDesc &answer) = 0;
	// ======================================================================
signals:
	void SignalCommand(const QCommandsService::SCmdDesc &cmd);
};

// ======================================================================

#endif // USE_COMMANDS_SERVICE

#endif // QCOMMANDSSERVICE_H
