#include "App/QMainClass.h"
#include "App/AppConfig.h"

#include "QThreadWorker.h"

// ======================================================================

QThreadWorker::QThreadWorker() : QObject(NULL)
{
	mThread = new QThread();

#ifdef USE_EPOOL_EVENT
	mThread->setEventDispatcher(new QEventDispatcherEpoll);
#endif // USE_EPOOL_EVENT
	
	this->moveToThread(mThread);

	connect(mThread, &QThread::started, this, &QThreadWorker::slotStarted);
	connect(mThread, &QThread::finished, this, &QThreadWorker::slotStopped);	
	connect(this, &QThreadWorker::signalError, QMainClass::getMainClass(), &QMainClass::slotWorkerError);	
}

QThreadWorker::~QThreadWorker()
{
	delete mThread;
}

// ======================================================================

/**
	* @brief  Запускает воркер и тред для него
	* @param  
	* @retval 
	*/
void QThreadWorker::StartWorker()
{
	mThread->start();
}

// ======================================================================

/**
	* @brief  Останавливает воркер и тред для него
	* @param  
	* @retval 
	*/
void QThreadWorker::StopWorker()
{
	mThread->quit();
	while (mThread->isRunning()) ;
}

// ======================================================================
