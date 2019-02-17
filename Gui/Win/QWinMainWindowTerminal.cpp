#include <QDebug>
#include <QFileDialog>
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
static const QString sFileName = "_file";
static const QString sFileChoice = "_file_choice";
static const QString sBtnSend = "_btn_send";
static const QString sBtnDel = "_btn_del";
// ======================================================================

QWinMainWindowTerminal::QWinMainWindowTerminal() :
	QMainWindow(),
	mUi(new Ui::QWinMainWindowTerminal)
{
	mUi->setupUi(this);
	
	// добавляем виджеты enabled которых зависит от состояния порта
	mListWidgetsGroup << mUi->btnRescanPorts
										<< mUi->listPorts
										<< mUi->listBaudrate
										<< mUi->listDataBits
										<< mUi->listParity
										<< mUi->listStopBits;
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
	rescanPorts();
	
	// изменяем состояние главного окна в зависимости от прочитаных настроек
	mUi->chAutoreconnect->setChecked(APP_SETTINGS()->TERMINAL.AUTO_RECONNECT);
	
	QWinMainWindowTerminal::setCurrentIndex(mUi->listPorts, APP_SETTINGS()->PORT.NAME);
	
	// заполняем список со значениями BaudRate
	foreach (QSerialPort::BaudRate baudRate, QComPortThread::sBaudRateList.keys()) {
		mUi->listBaudrate->addItem(QComPortThread::sBaudRateList[baudRate], baudRate);
	}
	QWinMainWindowTerminal::setCurrentIndex(mUi->listBaudrate, APP_SETTINGS()->PORT.BAUDRATE);
	
	// заполняем значения DataBits
	foreach (QSerialPort::DataBits dataBits, QComPortThread::sDataBitsList.keys()) {
		mUi->listDataBits->addItem(QComPortThread::sDataBitsList[dataBits], dataBits);
	}
	QWinMainWindowTerminal::setCurrentIndex(mUi->listDataBits, APP_SETTINGS()->PORT.DATABITS);

	// заполняем значения Parity
	foreach (QSerialPort::Parity parity, QComPortThread::sParityList.keys()) {
		mUi->listParity->addItem(QComPortThread::sParityList[parity], parity);
	}
	QWinMainWindowTerminal::setCurrentIndex(mUi->listParity, APP_SETTINGS()->PORT.PARITY);
	
	// заполняем список StopBits
	foreach (QSerialPort::StopBits stopBits, QComPortThread::sStopBitsList.keys()) {
		mUi->listStopBits->addItem(QComPortThread::sStopBitsList[stopBits], stopBits);
	}
	QWinMainWindowTerminal::setCurrentIndex(mUi->listStopBits, APP_SETTINGS()->PORT.STOPBITS);
	
	makeSignalSlots();
}

// ======================================================================

/**
	* @brief  Отображает главное окно
	* @param  None
	* @retval Void
	*/
void QWinMainWindowTerminal::show()
{
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
	settings.mPortName = mUi->listPorts->currentText();
	settings.mBoudRate =  (QSerialPort::BaudRate)mUi->listBaudrate->currentData().toUInt();
	settings.mDataBits = (QSerialPort::DataBits)mUi->listDataBits->currentData().toInt();
	settings.mParity = (QSerialPort::Parity)mUi->listParity->currentData().toInt();
	settings.mStopBits = (QSerialPort::StopBits)mUi->listStopBits->currentData().toInt();
	
	// FIXME mock settings
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
	bool btnAddCmdEnabled;
	bool btnAddFileEnabled;
	bool stateGoupWidgets;
	
	switch (newState)
	{
	case stsIdle:
		btnPortOpenCloseText = tr("Open");
		btnAddCmdEnabled = false;
		btnAddFileEnabled = false;
		stateGoupWidgets = true;
		break;
	// ======================================================================
	case stsConnecting:
		btnPortOpenCloseText = tr("Cancel opening");
		btnAddCmdEnabled = false;
		btnAddFileEnabled = false;
		stateGoupWidgets = false;
		break;
	// ======================================================================
	case stsConnected:
		btnPortOpenCloseText = tr("Close");
		btnAddCmdEnabled = true;
		btnAddFileEnabled = true;
		stateGoupWidgets = false;
		break;
	}
	
	mUi->btnPortOpenClose->setText(btnPortOpenCloseText);
	mUi->btnAddCmd->setEnabled(btnAddCmdEnabled);
	mUi->btnAddFile->setEnabled(btnAddFileEnabled);
	foreach (QWidget *widget, mListWidgetsGroup) {
		widget->setEnabled(stateGoupWidgets);
	}
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
	
	connect(mUi->chAutoreconnect, &QCheckBox::clicked,
					this, &QWinMainWindowTerminal::slotSettingsChanged);
	connect(mUi->listPorts, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this, &QWinMainWindowTerminal::slotSettingsChanged);
	connect(mUi->listBaudrate, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this, &QWinMainWindowTerminal::slotSettingsChanged);
	connect(mUi->listDataBits, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this, &QWinMainWindowTerminal::slotSettingsChanged);
	connect(mUi->listParity, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this, &QWinMainWindowTerminal::slotSettingsChanged);
	connect(mUi->listStopBits, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this, &QWinMainWindowTerminal::slotSettingsChanged);
	
	connect(mUi->btnAddCmd, &QPushButton::clicked,
					this, &QWinMainWindowTerminal::slotBtnAddRow);
	connect(mUi->btnAddFile, &QPushButton::clicked,
					this, &QWinMainWindowTerminal::slotBtnAddRow);
	
	connect(mUi->btnClear, &QPushButton::clicked,
					this, &QWinMainWindowTerminal::slotBtnClear);
}

// ======================================================================

/**
	* @brief  Обновляет список доступных портов
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::rescanPorts()
{
	QList<QSerialPortInfo> listPorts = MAIN_CLASS()->getPort()->getListPorts();
	
	mUi->listPorts->clear();
	foreach (QSerialPortInfo portInfo, listPorts) {
		mUi->listPorts->addItem(portInfo.portName());
	}
}

// ======================================================================

/**
	* @brief  Добавляет строку с данными команды для отправки
	* @param  uniqueName: уникальное имя, с которым будут создаться виджеты
	* @param  layout: слот на который будут добавляться виджеты
	* @retval 
	*/
void QWinMainWindowTerminal::addCommandRow(const QString &uniqueName, QBoxLayout *layout)
{
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
	
	connect(btnSend, &QPushButton::clicked, this, &QWinMainWindowTerminal::slotBtnSendCmd);
}

// ======================================================================

/**
	* @brief  Добавляет строку с отправкой содержимого файла в порт
	* @param  uniqueName: уникальное имя, с которым будут создаться виджеты
	* @param  layout: слот на который будут добавляться виджеты
	* @retval 
	*/
void QWinMainWindowTerminal::addFileRow(const QString &uniqueName, QBoxLayout *layout)
{
	// создаем поле для файла
	QLineEdit *fileCmd = new QLineEdit();
	fileCmd->setObjectName(uniqueName + sFileName);
	fileCmd->setReadOnly(true);
	layout->addWidget(fileCmd);
	
	// создаем кнопку для выбора файла
	QPushButton *btnFileChoice = new QPushButton();
	btnFileChoice->setObjectName(uniqueName + sFileChoice);
	btnFileChoice->setAccessibleName(uniqueName);
	btnFileChoice->setText(tr("Choose file"));
	layout->addWidget(btnFileChoice);
	
	// создаем кнопку для отправки файла
	QPushButton *btnSend = new QPushButton();
	btnSend->setObjectName(uniqueName + sBtnSend);
	btnSend->setAccessibleName(uniqueName);
	btnSend->setText(tr("Send file"));
	layout->addWidget(btnSend);

	// TODO тут возможна утечка памяти (см. https://wiki.qt.io/New_Signal_Slot_Syntax/ru)
	connect(btnFileChoice, &QPushButton::clicked, [fileCmd, this]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Choose file to send"));
		fileCmd->setText(fileName);
	});	
	connect(btnSend, &QPushButton::clicked, this, &QWinMainWindowTerminal::slotBtnSendFile);
}

// ======================================================================

/**
	* @brief  Меняет текущий индекс виджета QComboBox в зависимости от значения в userData
	*					или Text переданом в параметре value
	* @param  comboBox: виджет ComboBox, индекс которого нужно изменить
	* @param	value: значение для поиска в полях userData и text
	* @retval 
	*/
void QWinMainWindowTerminal::setCurrentIndex(QComboBox *comboBox, const QVariant &value)
{
	int i;
	
	i = comboBox->findData(value);
	if (i == -1) {
		i = comboBox->findText(value.toString());
	}
	
	if (i == -1) {
		return;
	}
	comboBox->setCurrentIndex(i);
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
void QWinMainWindowTerminal::slotBtnAddRow()
{
	// на главном окне несколько кнопок для создания "строк" команд.
	// можем добавить строку для текстовой команды, можем как файл
	// и чтобы не городить несколько обработчиков сигналов и слотов,
	// обработку всех кнопок повешали на один слот, в котором проверяется
	// тип добавляемой записи
	QWidget *w = (QWidget*)sender();
	if (!w) {
		qWarning() << "Widget sender is NULL!";
		return;
	}
	QString type = w->accessibleName();
	QString uniqueName = QUuid::createUuid().toString();
	
	// костыль для того, чтобы создать уникальное имя, но обрезав при этом фигурные скобки
	// из-за которых не получается найти всех элементов
	uniqueName = uniqueName.mid(1, uniqueName.length() - 2);
	
	// создаем слой, который будет содержать элементы для команды
	QHBoxLayout *layout = new QHBoxLayout();
	layout->setObjectName(uniqueName);
	// отдельные виджеты для конкретной реализации типа строки
	if (type == "command") {
		addCommandRow(uniqueName, layout);
	} else if (type == "file") {
		addFileRow(uniqueName, layout);
	}
	
	// создаем кнопку для удаление строки с командой
	// она общая для всех типов строк
	QPushButton *btnDel = new QPushButton();
	btnDel->setObjectName(uniqueName + sBtnDel);
	btnDel->setAccessibleName(uniqueName);
	btnDel->setText(tr("Delete command"));
	layout->addWidget(btnDel);
	
	// создаем связку слотов/сигналов
	connect(btnDel, &QPushButton::clicked, this, &QWinMainWindowTerminal::slotBtnDelCmd);
	
	mUi->listCmds->addLayout(layout);
}

// ======================================================================

/**
	* @brief  Смена настроек порта, приложения
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::slotSettingsChanged()
{
	APP_SETTINGS()->TERMINAL.AUTO_RECONNECT = mUi->chAutoreconnect->isChecked();
	
	APP_SETTINGS()->PORT.NAME = mUi->listPorts->currentText();
	APP_SETTINGS()->PORT.BAUDRATE = mUi->listBaudrate->currentData().toUInt();
	APP_SETTINGS()->PORT.DATABITS = mUi->listDataBits->currentData().toInt();
	APP_SETTINGS()->PORT.PARITY = mUi->listParity->currentData().toInt();
	APP_SETTINGS()->PORT.STOPBITS = mUi->listStopBits->currentData().toInt();
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
	* @brief  Клик кнопки "Send file". Отправка файла в порт
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::slotBtnSendFile()
{
	QWidget *btnSender = (QWidget*)sender();
	if (!btnSender) {
		qWarning() << "Sender in NULL!";
		return;
	}
	
	QString layoutName = btnSender->accessibleName();

	QLineEdit *fileName = findChild<QLineEdit*>(layoutName + sFileName);
	
	if (!fileName) {
		qWarning() << "Cannot find widget(s) in command row";
		return;
	}
	
	SFileSendDesc fileSend;
	fileSend.mFileName = fileName->text();

	MAIN_CLASS()->getTerminal()->sendFile(fileSend);
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

/**
	* @brief  Кнопка: очистить текст лога
	* @param  
	* @retval 
	*/
void QWinMainWindowTerminal::slotBtnClear()
{
	mUi->textData->clear();
}

// ======================================================================
