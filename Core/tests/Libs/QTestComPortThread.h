/**
	* @version  
	* @brief    Юнит тест для проверки класса QComPortThread
	*/
#ifndef QTESTCOMPORTTHREAD_H
#define QTESTCOMPORTTHREAD_H

#include <QObject>
#include <QSignalSpy>

#include "Core/Libs/QComPortThread.h"

class QTestComPortThread : public QObject
{
	Q_OBJECT

public:
	explicit QTestComPortThread(QObject *parent = 0);
	
// ======================================================================
	
private slots:
	void testOpenLostPort();
	void testReadData();
	
// ======================================================================
	
private:
	QComPortThread *mPort;
	
};

#endif // QTESTCOMPORTTHREAD_H
