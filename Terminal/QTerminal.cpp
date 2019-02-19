#include <QDebug>
#include <QRegExp>
#include <QFile>

#include "Core/Utils/QStringUtils.h"

#include "QTerminal.h"

#include "App/QMainClass.h"

// ======================================================================

// таблица с состояниями приложения
#define TABLE_MAKE()	\
	TABLE_BEGIN	\
	TABLE_BODY0(stIdle)	\
	TABLE_BODY(stConnecting)	\
	TABLE_BODY(stConnected)	\
	TABLE_END

// ======================================================================
//  stIdle
//	Порт закрыт. 
//	Ожидаем команду от пользователя.
// ======================================================================
//	stConnecting
//	Пытаемся открыть порт.
// ======================================================================
//	stConnected
//	Порт открыт.
// ======================================================================


// ======================================================================

#define TABLE_BEGIN					enum {
#define TABLE_BODY0(name)			name = QStateMachineEx::_stStartup,
#define TABLE_BODY(name)			name,
#define TABLE_END						STATES_COUNT};

// создаем Enum'ы 
TABLE_MAKE()

// ======================================================================

#undef TABLE_BEGIN					
#undef TABLE_BODY0
#undef TABLE_BODY
#undef TABLE_END

#define TABLE_BEGIN					static const QStateMachineEx::TStateName sStatesList = {
#define TABLE_BODY0(name)			{name, #name},
#define TABLE_BODY(name)			{name, #name},
#define TABLE_END						};

// создаем таблицу с именами статусов
TABLE_MAKE()

// ======================================================================

QTerminal::QTerminal(QObject *parent) : 
	QStateMachineEx("Terminal", sStatesList, parent)
{
}

// ======================================================================

QTerminal::~QTerminal()
{
}

// ======================================================================

// ======================================================================
//  public                       
// ======================================================================

/**
	* @brief  Инициализация класса
	* @param  
	* @retval 
	*/
void QTerminal::init()
{
	mPort = MAIN_CLASS()->getPort();
	mGui = MAIN_CLASS()->getGui();
	
	connect(mPort, &QComPortThread::signalResultOpen, this, &QTerminal::slotPortResultOpen);
	connect(mPort, &QComPortThread::signalClose, this, &QTerminal::slotPortClose);
	connect(mPort, &QComPortThread::signalIncomingData, this, &QTerminal::slotReadData);
	
	// переход в начальное состояние
	setStateIdle();
}

// ======================================================================

/**
	* @brief  Обработка события от главного окна открытия/закрытия порта
	*					в зависимости от его текущего состояния
	* @param  
	* @retval 
	*/
void QTerminal::portOpenClose()
{
	switch (getState())
	{
	/// пытаемся открыть порт
	case stIdle:
		setStateConnecting();
		break;
	// ======================================================================
	/// отменяем попытку подключиться
	case stConnecting:
		setStateIdle(); 
		break;
	// ======================================================================
	/// закрываем порт
	case stConnected:
		mPort->close();
		mGui->writeLog(tr("Closed the port"));
		setStateIdle(); 
		break;
	// ======================================================================
	default:
		qWarning() << "Unknown reaction for state: " << getState();
	} // switch
}

// ======================================================================

/**
	* @brief  Отправка команды в порт
	* @param  cmd: структура с командой
	* @retval 
	*/
void QTerminal::sendCommand(const SCommandDesc &cmd)
{
	if (getState() != stConnected) {
		qWarning() << "Send command when port is not opened. It's not expected behavior!";
		return;
	}
	
	if (cmd.mIsHex && !QStringUtils::isValidHex(cmd.mCmdData)) {
		qDebug() << "Data is not hex!";
		// TODO передача сообщения об ошибке в главное окно
		return;
	}
	
	QByteArray dataToSend = cmd.mIsHex ? QByteArray::fromHex(cmd.mCmdData) : cmd.mCmdData;
	if (cmd.mRn) {
		dataToSend.append("\r\n");
	}
	mPort->sendData(dataToSend);
}

// ======================================================================

/**
	* @brief  Отправка содержимого файла в порт
	* @param  fileSend: структура описывающая файл для отправки
	* @retval 
	*/
void QTerminal::sendFile(const SFileSendDesc &fileSend)
{
	QFile file(fileSend.mFileName);
	
	if (!file.exists()) {
		qDebug() << "File does not exist";
		// TODO передача сообщения об ошибке в главное окно
		return;
	}
	
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "Error open file";
		// TODO передача сообщения об ошибке в главное окно
		return;
	}
	
	// TODO тут возможно понадобится чтение не всего файла сразу, а по частям
	// для экономии памяти
	QByteArray data = file.readAll();
	mPort->sendData(data);
}

// ======================================================================

// ======================================================================
//  protected                       
// ======================================================================

/**
	* @brief  Таймаут таймера
	* @param  
	* @retval 
	*/
void QTerminal::onEvTimer()
{
	switch (getState())
	{
	case stConnecting:
		mPort->open();
		break;
	// ======================================================================
	default:
		break;
	}
}

// ======================================================================

// ======================================================================
//  states                       
// ======================================================================

/**
	* @brief  Состояние: ожидание
	* @param  
	* @retval 
	*/
void QTerminal::setStateIdle()
{
	setState(stIdle);
	
	// сбрасываем состояние главного окна
	mGui->setStateWindow(IGui::stsIdle);
}

// ======================================================================

/**
	* @brief  Состояние: подключение
	*					Пытаемся открыть порт
	* @param  
	* @retval 
	*/
void QTerminal::setStateConnecting()
{
	setState(stConnecting);
	
	// меняем настройки порта
	QComPortThread::SSettings settings = MAIN_CLASS()->getGui()->getPortSettings();
	mPort->setSettings(settings);
	setTimer(0);
	
	// меняем состояние главного окна
	mGui->setStateWindow(IGui::stsConnecting);
}

// ======================================================================

/**
	* @brief  Состояние: подключено
	*					Порт открыт, нужно поменять состояние главное окна
	* @param  
	* @retval 
	*/
void QTerminal::setStateConnected()
{
	setState(stConnected);

	// меняем состояние главного окна
	mGui->setStateWindow(IGui::stsConnected);
}

// ======================================================================

// ======================================================================
//  protected slots                       
// ======================================================================

/**
	* @brief  Обработка результата открытия порта
	* @param  
	* @retval 
	*/
void QTerminal::slotPortResultOpen(bool res)
{
	mGui->writeLog(tr("Result open port"), res ? EResultCodes::resOk : EResultCodes::resError);
	if (!res) {
		// порт не был открыт, повторяем открытие порта через 1сек.
		setTimer(TIMEOUT_TRY_OPEN_PORT);
		return;
	}
	setStateConnected();
}

// ======================================================================

/**
	* @brief  Слот для обработки внезапного отключения порта
	* @param  
	* @retval 
	*/
void QTerminal::slotPortClose()
{
	qDebug() << "..." << getStateStr();
	// TODO проверить логику работы с отключением устройства
	
	switch (getState()) {
	// ======================================================================
	case stConnected:
		// если в настройках стоит автоматический реконект, то пытаемся снова открыть порт
		if (APP_SETTINGS()->TERMINAL.AUTO_RECONNECT) {
			setStateConnecting();
		} else {
			setStateIdle(); 
		}
		break;
	// ======================================================================
	default:
		break;
	}
}

// ======================================================================

/**
	* @brief  Слот для обработки входящих данных из порта
	* @param  
	* @retval 
	*/
void QTerminal::slotReadData(const QByteArray &data)
{
	if (getState() != stConnected) {
		qWarning() << "Incoming data when port is not opened. It's not expected behavior!";
		return;
	}
	
//	qDebug() << "Got data. Length: " << data.length();
	mGui->addData(data);
}

// ======================================================================
