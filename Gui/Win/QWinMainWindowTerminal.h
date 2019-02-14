/**
	* @version  
	* @brief    Класс для работы с главный окном приложения для Windows систем
	*/
#ifndef QWINMAINWINDOWTERMINAL_H
#define QWINMAINWINDOWTERMINAL_H

#include <QMainWindow>
#include <QBoxLayout>

#include "Collections/Interfaces.h"

namespace Ui {
class QWinMainWindowTerminal;
}

class QWinMainWindowTerminal : public QMainWindow, public IGui
{
	Q_OBJECT
	
public:
	explicit QWinMainWindowTerminal();
	~QWinMainWindowTerminal();
	// ======================================================================
	void init();
	void show();
	QComPortThread::SSettings getPortSettings();
	void setStateWindow(EWindowState newState);
	void addData(const QByteArray &data);
	// ======================================================================
	void writeLog(const QString &text);
	void writeLog(const QString &text, EResultCodes res);
	
// ======================================================================
	
protected:
	void makeSignalSlots();
	void rescanPorts();
	void addCommandRow(const QString &uniqueName, QBoxLayout *layout);
	void addFileRow(const QString &uniqueName, QBoxLayout *layout);
	
// ======================================================================
	
protected:
	Ui::QWinMainWindowTerminal *mUi;
	
// ======================================================================
	
protected slots:
	void slotBtnPortOpenClose();
	void slotBtnRescanPorts();
	void slotBtnAddRow();
	void slotChAutoreconnect();
	void slotBtnSendCmd();
	void slotBtnSendFile();
	void slotBtnDelCmd();
	
	
// ======================================================================
	
};

#endif // QWINMAINWINDOWTERMINAL_H
