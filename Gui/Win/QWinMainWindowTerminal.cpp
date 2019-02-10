#include <QDebug>
#include <QRegularExpression>
#include <QUuid>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>

#include "QWinMainWindowTerminal.h"
#include "ui_QWinMainWindowTerminal.h"

#include "App/QMainClass.h"

// ======================================================================
// текстовое представление кодов результатов выполнения
static QHash<EResultCodes, QString> sResultCodeText = {
	{EResultCodes::resOk, QObject::tr("Ok")},
	{EResultCodes::resFail, QObject::tr("Fail")},
	{EResultCodes::resError, QObject::tr("Error")}
};
// ======================================================================
// постфиксы ждя виджетов, текст команды, кнопки и т.п
static const QString sCmdText = "_text";
static const QString sChRn = "_ch_rn";
static const QString sChHex = "_ch_hex";
static const QString sBtnSend = "_btn_send";
static const QString sBtnDel = "_btn_del";
// ======================================================================

QWinMainWindowTerminal::QWinMainWindowTerminal() :
	QMainWindow(),
	mUi(new Ui::QWinMainWindowTerminal)
{
	mUi->setupUi(this);
}

// ======================================================================

QWinMainWindowTerminal::~QWinMainWindowTerminal()
{
	delete mUi;
}

// ======================================================================

// ======================================================================
//  public                       
// ======================================================================

/**
	* @brief  Инициалиирует класс для работы с главным окном
	* @param  None
	* @retval Void
	*/
void QWinMainWindowTerminal::init()
{
	makeSignalSlots();	
	
	// изменяем состояние главного окна в зависимости от прочитаных настроек
	mUi->chAutoreconnect->setChecked(APP_SETTINGS()->TERMINAL.AUTO_RECONNECT);
}

// ======================================================================

/**
	* @brief  Отображает главное окно
	* @param  None
	* @retval Void
	*/
void QWinMainWindowTerminal::show()
{
	rescanPorts();
	QMainWindow::show();
}

// ======================================================================

/**
	* @brief  Возвращает структуру с настройками порта из пользовательского интерфейса
	* @param  None
	* @retval QComPortThread::SSettings: структура с настройками порта
	*/
QComPortThread::SSettings QWinMainWindowTerminal::getPortSettings()
{
	QComPortThread::SSettings settings;
	settings.mPortName = mUi->portsList->currentText();
	
	// FIXME mock settings
	settings.mBoudRate = (QSerialPort::BaudRate)115200;
	settings.mDataBits = (QSerialPort::DataBits)8;
	settings.mParity = (QSerialPort::Parity)0;
	settings.mStopBits = (QSerialPort::StopBits)1;
	settings.mFlowControl = (QSerialPort::FlowControl)0;
	
	return settings;
}

// ======================================================================

/**
	* @brief  Меняет состояние кнопки открытия/закрытия порта
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::setStateWindow(EWindowState newState)
{
	QString btnPortOpenCloseText;
	bool btnRescanPortsEnabled;
	bool btnAddCmdEnabled;
	
	switch (newState)
	{
	case stsIdle:
		btnPortOpenCloseText = tr("Open");
		btnRescanPortsEnabled = true;
		btnAddCmdEnabled = false;
		break;
	// ======================================================================
	case stsConnecting:
		btnPortOpenCloseText = tr("Opening");
		btnRescanPortsEnabled = false;
		btnAddCmdEnabled = false;
		break;
	// ======================================================================
	case stsConnected:
		btnPortOpenCloseText = tr("Close");
		btnRescanPortsEnabled = false;
		btnAddCmdEnabled = true;
		break;
	}
	
	mUi->btnPortOpenClose->setText(btnPortOpenCloseText);
	mUi->btnRescanPorts->setEnabled(btnRescanPortsEnabled);
	mUi->btnAddCmd->setEnabled(btnAddCmdEnabled);
}

// ======================================================================

/**
	* @brief  Добавляет получеые из порта данные
	* @param  data: массив с даннами
	* @retval void
	*/
void QWinMainWindowTerminal::addData(const QByteArray &data)
{
	QString str;
	
	// TODO продумать свпособ по лучше передавать данные
	foreach (quint8 c, data) {
		str.append(c);
	}

	mUi->textData->insertPlainText(str);
	mUi->textData->moveCursor(QTextCursor::End);
}

// ======================================================================

/**
	* @brief  Записывает информация в журнал лога
	* @param  text: текст для записи
	* @retval 
	*/
void QWinMainWindowTerminal::writeLog(const QString &text)
{
	mUi->log->appendPlainText(text);
}

// ======================================================================

/**
	* @brief  Записывает информацию в журнал лога с результатом выполнения
	* @param  text: строка для записи
	* @param  res: результат для записи в лог
	* @retval 
	*/
void QWinMainWindowTerminal::writeLog(const QString &text, EResultCodes res)
{
	QString logText = QString("%1: %2").arg(text).arg(sResultCodeText[res]);
	mUi->log->appendPlainText(logText);
}

// ======================================================================

// ======================================================================
//  protected                       
// ======================================================================

/**
	* @brief  Создает свящку сигналов/слотов
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::makeSignalSlots()
{
	connect(mUi->btnPortOpenClose, &QPushButton::clicked,
					this, &QWinMainWindowTerminal::slotBtnPortOpenClose);
	connect(mUi->btnRescanPorts, &QPushButton::clicked,
					this, &QWinMainWindowTerminal::slotBtnRescanPorts);
	connect(mUi->btnAddCmd, &QPushButton::clicked,
					this, &QWinMainWindowTerminal::slotBtnAddCmd);
	connect(mUi->chAutoreconnect, &QCheckBox::clicked,
					this, &QWinMainWindowTerminal::slotChAutoreconnect);
}

// ======================================================================

/**
	* @brief  Обновляет список доступных портов
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::rescanPorts()
{
	QList<QSerialPortInfo> portsList = MAIN_CLASS()->getPort()->getPortsList();
	
	mUi->portsList->clear();
	foreach (QSerialPortInfo portInfo, portsList) {
		mUi->portsList->addItem(portInfo.portName());
	}
}

// ======================================================================

// ======================================================================
//  protected slots                       
// ======================================================================

/**
	* @brief  Обработки клика на кнопку "Открыть порт"
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::slotBtnPortOpenClose()
{
	MAIN_CLASS()->getTerminal()->portOpenClose();
}

// ======================================================================

/**
	* @brief  Клик "Rescan ports"
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::slotBtnRescanPorts()
{
	rescanPorts();
}

// ======================================================================

/**
	* @brief  Клик "Add command"
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::slotBtnAddCmd()
{
	QString uniqueName = QUuid::createUuid().toString();
	
	// костыль для того, чтобы создать уникальное имя, но обрезав при этом фигурные скобки
	// из-за которых не получается найти всех элементов
	uniqueName = uniqueName.mid(1, uniqueName.length() - 2);
	
	// создаем слой, который будет содержать элементы для команды
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setObjectName(uniqueName);
	
	// создаем поле для ввода команды
	QLineEdit *cmdText = new QLineEdit();
	cmdText->setObjectName(uniqueName + sCmdText);
	layout->addWidget(cmdText);
	
	// создаем чек-бокс для добавления перевода строки и возврата коретки
	QCheckBox *chRn = new QCheckBox();
	chRn->setObjectName(uniqueName + sChRn);
	chRn->setChecked(false);
	chRn->setText(tr("Add \\r\\n"));
	layout->addWidget(chRn);

	// создаем чек-бокс для HEX представления 
	QCheckBox *chHex = new QCheckBox();
	chHex->setObjectName(uniqueName + sChHex);
	chHex->setChecked(false);
	chHex->setText(tr("HEX"));
	layout->addWidget(chHex);
	
	// создаем кнопку для отправки команды
	QPushButton *btnSend = new QPushButton();
	btnSend->setObjectName(uniqueName + sBtnSend);
	btnSend->setAccessibleName(uniqueName);
	btnSend->setText(tr("Send command"));
	layout->addWidget(btnSend);
	
	// создаем кнопку для удаление строки с командой
	QPushButton *btnDel = new QPushButton();
	btnDel->setObjectName(uniqueName + sBtnDel);
	btnDel->setAccessibleName(uniqueName);
	btnDel->setText(tr("Delete command"));
	layout->addWidget(btnDel);
	
	// создаем связку слотов/сигналов
	connect(btnSend, &QPushButton::clicked, this, &QWinMainWindowTerminal::slotBtnSendCmd);
	connect(btnDel, &QPushButton::clicked, this, &QWinMainWindowTerminal::slotBtnDelCmd);
	
	mUi->listCmds->addLayout(layout);
}

// ======================================================================

/**
	* @brief  Смена состояния чек бокса auto_reconnect
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::slotChAutoreconnect()
{
	APP_SETTINGS()->TERMINAL.AUTO_RECONNECT = mUi->chAutoreconnect->isChecked();
}

// ======================================================================

/**
	* @brief  Клик кнопки "Send command". Кнопки были созданны динамически
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::slotBtnSendCmd()
{
	QWidget *btnSender = (QWidget*)sender();
	if (!btnSender) {
		qWarning() << "Sender in NULL!";
		return;
	}
	
	QString layoutName = btnSender->accessibleName();

	QLineEdit *cmdText = findChild<QLineEdit*>(layoutName + sCmdText);
	QCheckBox *chRn = findChild<QCheckBox*>(layoutName + sChRn);
	QCheckBox *chHex = findChild<QCheckBox*>(layoutName + sChHex);
	
	if (!cmdText || !chRn || !chHex) {
		qWarning() << "Cannot find widget(s) in command row";
		return;
	}
	
	SCommandDesc cmd;
	cmd.mCmdData = cmdText->text().toUtf8();
	cmd.mRn = chRn->isChecked();
	cmd.mIsHex = chHex->isChecked();

	MAIN_CLASS()->getTerminal()->sendCommand(cmd);
}

// ======================================================================

/**
	* @brief  Клик кнопки "Del". Удаление строки с командой
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::slotBtnDelCmd()
{
	QWidget *btnSender = (QWidget*)sender();
	if (!btnSender) {
		qWarning() << "Sender in NULL!";
		return;
	}

	QString layoutName = btnSender->accessibleName();
	
	QHBoxLayout *layout = findChild<QHBoxLayout*>(layoutName);
	if (!layout) {
		qWarning() << "Cannot find layout with name: " << layoutName;
		return;
	}

	// регулярка для поиска всех элементов строки команды
	QRegularExpression reg(QString("%1\\S+").arg(layoutName));
	
	// т.к layout не может быть родителем QWidget элементов, то перебираем их всех и удаляем
	QList<QWidget*> children = findChildren<QWidget*>(reg);
	foreach (QWidget *widget, children) {
		qDebug() << "Delete: " << widget->objectName();
		widget->deleteLater();
	}
	layout->deleteLater();
}

// ======================================================================
