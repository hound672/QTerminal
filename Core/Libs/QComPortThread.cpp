/**
  * @version  1.0
  * @brief    Класс для работы с COM портом в отдельном потоке
  */
#include <QDebug>
#include <QDateTime>

#include "Core/Libs/QComPortThread.h"

// ======================================================================
const QMap<QSerialPort::BaudRate, QString> QComPortThread::sBaudRateList = {
	{QSerialPort::Baud1200, "1200"},
	{QSerialPort::Baud2400, "2400"},
	{QSerialPort::Baud4800, "4800"},
	{QSerialPort::Baud9600, "9600"},
	{QSerialPort::Baud19200, "19200"},
	{QSerialPort::Baud38400, "38400"},
	{QSerialPort::Baud57600, "57600"},
	{QSerialPort::Baud115200, "115200"},
};

const QMap<QSerialPort::DataBits, QString> QComPortThread::sDataBitsList = {
	{QSerialPort::Data5, "5"},
	{QSerialPort::Data6, "6"},
	{QSerialPort::Data7, "7"},
	{QSerialPort::Data8, "8"},
};
	
const QMap<QSerialPort::Parity, QString> QComPortThread::sParityList = {
	{QSerialPort::NoParity, "None"},
	{QSerialPort::EvenParity, "Even"},
	{QSerialPort::OddParity, "Odd"},
	{QSerialPort::SpaceParity, "Space"},
	{QSerialPort::MarkParity, "Mark"},
};

const QMap<QSerialPort::StopBits, QString> QComPortThread::sStopBitsList = {
	{QSerialPort::OneStop, "1"},
	{QSerialPort::OneAndHalfStop, "1.5"},
	{QSerialPort::TwoStop, "2"},
};
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
QList<QSerialPortInfo> QComPortThread::getListPorts() const
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
									<< ". BaudRate: " << mPort->baudRate()
									<< ". DataBits: " << mPort->dataBits()
									<< ". Parity: " << mPort->parity()
									<< ". StopBits: " << mPort->stopBits()
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

