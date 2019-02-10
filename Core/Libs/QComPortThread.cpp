/**
  * @version  1.0
  * @brief    Класс для работы с COM портом в отдельном потоке
  */
#include <QDebug>
#include <QDateTime>

#include "Core/Libs/QComPortThread.h"

// ======================================================================

/**
  * @brief  Конструктор класса
  * @param
  * @retval
  */
QComPortThread::QComPortThread() :
  QThreadWorker()
{
	qRegisterMetaType<QComPortThread::SSettings>("QComPortThread::SSettings");
}

// ======================================================================

QComPortThread::~QComPortThread()
{
}

// ======================================================================

// ======================================================================
//  public                        
// ======================================================================

/**
	* @brief  Смена настроек порта
	* @param  settings: настройки порта
	* @retval void
	*/
void QComPortThread::setSettings(const QComPortThread::SSettings &settings)
{
	QMetaObject::invokeMethod(this, "slotSetSettings", 
														Q_ARG(const QComPortThread::SSettings, settings));
}

// ======================================================================

/**
  * @brief  Вызывает слот для открытия порта
  * @param  namePort: имя ком порта в системе
  * @retval void
  */
void QComPortThread::open()
{
  QMetaObject::invokeMethod(this, "slotOpen");
}

// ======================================================================

/**
  * @brief  Вызывает слот для закрытия ком порта
  * @param
  * @retval
  */
void QComPortThread::close()
{
  QMetaObject::invokeMethod(this, "slotClose");
}

// ======================================================================

/**
  * @brief  Отправляет данные в текущий ком порт
  * @param  data: данные для отправки
  * @retval void
  */
void QComPortThread::sendData(const QByteArray &data)
{
  QMetaObject::invokeMethod(this, "slotSendData", 
														Q_ARG(const QByteArray&, data));
}

// ======================================================================

/**
	* @brief  Возвращает список доступных портов
	* @param  
	* @retval 
	*/
QList<QSerialPortInfo> QComPortThread::getPortsList() const
{
	return QSerialPortInfo::availablePorts();
}

// ======================================================================

/**
  * @brief  Проверяыет на доступность текущий ком порт
  * @param  None
  * @retval true: если порт доступен, иначе false
  */
bool QComPortThread::checkPort() const
{ 
  QSerialPortInfo portInfo(mPort->portName());
	return !portInfo.isNull();
}

// ======================================================================
//  protected slots
// ======================================================================

// ======================================================================

/**
	* @brief  Слот для обработки запуска воркера
	* @param  None
	* @retval void
	*/
void QComPortThread::slotStarted()
{  
	mPort = new QSerialPort();
 
	// связывание сигналов слотов для ком порта
	qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");
	connect(mPort, &QSerialPort::readyRead, this, &QComPortThread::slotReadData);

	mTimer = new QTimer();
	connect(mTimer, &QTimer::timeout, this, &QComPortThread::slotTimeout);
	mTimer->start(TIMER_TIMEOUT);
}

// ======================================================================

/**
	* @brief  Слот для обработки остановки работы воркера
	* @param  None
	* @retval void
	*/
void QComPortThread::slotStopped()
{
	if (mPort->isOpen()) {
		mPort->close();
	}
  delete mPort;
}

// ======================================================================

/**
	* @brief  Слот для обработки тика таймера
	* @param  None
	* @retval void
	*/
void QComPortThread::slotTimeout()
{
	// по таймеру при открытом порту проверяем его доступность
	// если порт не доступен, значит порт был потерян системой
	if (mPort->isOpen() && !checkPort()) {
		qDebugComPort() << "Port has been lost. Clost it";
		slotClose();
	}
}

// ======================================================================

/**
	* @brief  Слот для обработки смены настроек порта
	* @param  settings: структура с настройками
	* @retval void
	*/
void QComPortThread::slotSetSettings(const QComPortThread::SSettings &settings)
{
	mPort->setPortName(settings.mPortName);
	mPort->setBaudRate(settings.mBoudRate);
	mPort->setDataBits(settings.mDataBits);
	mPort->setParity(settings.mParity);
	mPort->setStopBits(settings.mStopBits);
	mPort->setFlowControl(settings.mFlowControl);
}

// ======================================================================

/**
  * @brief  Открывает ком порт
  * @param	None
  * @retval	void
  */
void QComPortThread::slotOpen()
{
  if (mPort->isOpen()) {
		qWarning() << "Port is already open!";
    return;
  }

	bool res = mPort->open(QIODevice::ReadWrite);
	emit signalResultOpen(res);
	qDebugComPort() << "Port: " << mPort->portName()
									<< ". Result open: " << res;
}

// ======================================================================

/**
  * @brief  Приостанавливает работу. Закрывает порт.
  * @param
  * @retval
  */
void QComPortThread::slotClose()
{
	if (!mPort->isOpen()) {
		return;
	}
	
  mPort->close();
  emit signalClose();
}

// ======================================================================

/**
  * @brief  Отправляем данные в ком порт
  * @param
  * @retval
  */
void QComPortThread::slotSendData(const QByteArray &data)
{
  if (!mPort->isOpen()) {
    return;
  }

	mPort->write(data);
}

// ======================================================================

/**
  * @brief  Слот для приема сообщений от ком порта
  * @param  None
  * @retval void
  */
void QComPortThread::slotReadData()
{
  QByteArray data = this->mPort->readAll();
  emit signalIncomingData(data);
}

// ======================================================================

