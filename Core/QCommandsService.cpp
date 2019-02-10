#ifdef USE_COMMANDS_SERVICE

#include <QDebug>

#include "App/QMainClass.h"

#include "QCommandsService.h"

// ======================================================================

QCommandsService::QCommandsService(ICommandsClient *client) : 
	QThreadWorker(),
	mClient(client)
{
	qRegisterMetaType<SCmdDesc>("SCmdDesc");
	qRegisterMetaType<QCommandsService::SCmdDesc>("QCommandsService::SCmdDesc");
	qRegisterMetaType<SAnswerDesc>("SAnswerDesc");
	qRegisterMetaType<QCommandsService::SAnswerDesc>("QCommandsService::SAnswerDesc");
	
	mClient->moveToThread(mThread);
	connect(mClient, &ICommandsClient::SignalCommand, this, &QCommandsService::SlotGotCommandFromClient);
}

// ======================================================================

QCommandsService::~QCommandsService()
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
void QCommandsService::slotStarted()
{
	int res;
	
	res = mClient->Init();
	if (res < 0) {
		qCritical("Error init Commands Client: %d", res);
		WORKER_ERROR();
	}
	
	qInfo("Commands service is started");
}

// ======================================================================

/**
	* @brief  Остановка воркера
	* @param  
	* @retval 
	*/
void QCommandsService::slotStopped()
{
	qInfo("Commands service is stopped");
}

// ======================================================================

/**
	* @brief  Слот получения команды от клиента
	* @param  
	* @retval 
	*/
void QCommandsService::SlotGotCommandFromClient(const SCmdDesc &cmd)
{
	emit SignalGotCommand(cmd);
}

// ======================================================================

// ======================================================================
//  public slots                       
// ======================================================================

/**
	* @brief  Слот для обработки полученого ответа
	* @param  
	* @retval 
	*/
void QCommandsService::slotSendAnswer(const SAnswerDesc &answer)
{
	qCmdDebug("Send answer. Res: %d", answer.mRes);
	mClient->SendAnswer(answer);
}

// ======================================================================

#endif // USE_COMMANDS_SERVICE
