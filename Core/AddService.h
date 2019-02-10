#ifndef ADDSERVICE_H
#define ADDSERVICE_H

#include "App/QMainClass.h"

// ======================================================================

#ifdef USE_MONITORING_SERVICE
	// макрос для отправки в воркер мониторинга
	#define MONITORING_STATE(type, val) \
		QMetaObject::invokeMethod(gMainClass->GetMonitorService(), "slotMonitoring", \
															Q_ARG(QMonitoringService::EMonTypes, type),\
															Q_ARG(const QVariant &, val));

#else // USE_MONITORING_SERVICE
	#define MONITORING_STATE(type, val)
#endif // USE_MONITORING_SERVICE

// ======================================================================

#ifdef USE_COMMANDS_SERVICE
	// макрос для отправки ответа на команду
	#define COMMAND_ANSWER(answer)	\
		QMetaObject::invokeMethod(gMainClass->GetCommandsService(), "slotSendAnswer", \
															Q_ARG(const QCommandsService::SAnswerDesc&, answer));
	// ======================================================================
	// макрос для подписки класса на прием команд (прописывается в конструкторе)
	#define COMMANDS_SUBSCRIBE(CLASS)	\
		connect(gMainClass->GetCommandsService(), &QCommandsService::SignalGotCommand,	\
						this, &CLASS::slotCommandGot)
		
#else // USE_COMMANDS_SERVICE
	#define COMMAND_ANSWER(...)
	#define COMMANDS_SUBSCRIBE(...)
#endif // USE_COMMANDS_SERVICE

// ======================================================================


#endif // ADDSERVICE_H
