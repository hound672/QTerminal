#include <QDebug>
#include <QTest>
#include <QThread>

#include "QTestComPortThread.h"

// ======================================================================
// имя порта для тестов, должно быть реальное внешнее устройство
static QString sPortNameDevice = "COM6"; 
// имя порта эмулятора ком порта
static QString sPortNameMock = "COM1";
// ======================================================================

QTestComPortThread::QTestComPortThread(QObject *parent) : QObject(parent)
{
	mPort = new QComPortThread();
	mPort->StartWorker();
	
	
}

// ======================================================================

// ======================================================================
//  puvate slots                       
// ======================================================================

/**
	* @brief  Тест на открытие порта и потом на его "потерю".
	*					Необходимо какое-либо внешнее устройство для физического подключения 
	*					и отключения устройства от порта.
	* @param  
	* @retval 
	*/
void QTestComPortThread::testOpenLostPort()
{
	QSignalSpy *spyConnect = new QSignalSpy(mPort, SIGNAL(signalResultOpen(bool)));
	QSignalSpy *spyDisconnect = new QSignalSpy(mPort, SIGNAL(signalClose()));
	QComPortThread::SSettings settings = {sPortNameDevice};
	
	mPort->setSettings(settings);
	
	qDebug() << "Please, connect device to PC";
	
	// даем 3 сек. на подключение устройства
	QThread::msleep(3000);
	mPort->open();
	QVERIFY(spyConnect->wait(5000));
	bool res = qvariant_cast<bool>(spyConnect->at(0).at(0));
	QCOMPARE(res, true);
	
	qDebug() << "Please, disconnect device from PC";
	// даем 3 сек. на отключение устройства
	QVERIFY(spyDisconnect->wait(5000));
}

// ======================================================================

/**
	* @brief  Тест на прием данных через эмулятор порта
	* @param  
	* @retval 
	*/
void QTestComPortThread::testReadData()
{
	QSignalSpy *spyRead = new QSignalSpy(mPort, SIGNAL(signalIncomingData(const QByteArray&)));
	QComPortThread::SSettings settings = {sPortNameMock};
	
	mPort->setSettings(settings);
	
	mPort->open();
	qDebug() << "Send data: HELLO to virtual COM port";
	QVERIFY(spyRead->wait(5000));
	QByteArray data = qvariant_cast<QByteArray>(spyRead->at(0).at(0));
	QCOMPARE(data, QByteArray("HELLO"));
	
	qDebug() << "In terminal will have to be data: QWERTY";
	mPort->sendData(QByteArray("QWERTY"));
}

// ======================================================================
