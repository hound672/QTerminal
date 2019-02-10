/**
  * @version  
  * @brief    Класс для воркера работающего в отдельном треде
  */
#ifndef QTHREADWORKER_H
#define QTHREADWORKER_H

#include <QObject>
#include <QThread>

#include "Core/QEventDispatcherEpoll/qeventdispatcher_epoll.h"

#define WORKER_ERROR() emit signalError(-__LINE__);return;

class QThreadWorker : public QObject
{
	Q_OBJECT
public:
	explicit QThreadWorker();
	virtual ~QThreadWorker();
	void StartWorker();
	void StopWorker();	
	
// ======================================================================
	
protected:
	QThread *mThread;
	
// ======================================================================
	
protected slots:
	virtual void slotStarted() = 0;
	virtual void slotStopped() = 0;
	
// ======================================================================
	
signals:
	void signalError(int err);
	
// ======================================================================
	
};

#endif // QTHREADWORKER_H
