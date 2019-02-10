/**
	* @version  
	* @brief    Класс организующий работу с логикой работы приложения
	*/
#ifndef QTERMINAL_H
#define QTERMINAL_H

#include <QObject>
#include <QTimer>

#include "Core/QStateMachineEx.h"
#include "Core/Libs/QComPortThread.h"

#include "Collections/Interfaces.h"

class QTerminal : public QStateMachineEx
{
	Q_OBJECT
	
protected:
	enum {
		TIMEOUT_TRY_OPEN_PORT = 1000,	// время между попытками открыть порт
	};
	
// ======================================================================
	
public:
	explicit QTerminal(QObject *parent = 0);
	~QTerminal();
	// ======================================================================
	void init();
	void portOpenClose();
	void sendCommand(const SCommandDesc &cmd);
	
// ======================================================================
	
protected:
	QComPortThread *mPort;
	IGui *mGui;
	
// ======================================================================
	
protected:
	void onEvTimer();
	// ======================================================================
	void setStateIdle();
	void setStateConnecting();
	void setStateConnected();
	
// ======================================================================
	
protected slots:
	void slotPortResultOpen(bool res);
	void slotPortClose();
	void slotReadData(const QByteArray &data);

};

#endif // QTERMINAL_H
