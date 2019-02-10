/**
	* @version  
	* @brief    Класс для работы с главный окном приложения для Windows систем
	*/
#ifndef QWINMAINWINDOWTERMINAL_H
#define QWINMAINWINDOWTERMINAL_H

#include <QMainWindow>

#include "Collections/Interfaces.h"

namespace Ui {
class QWinMainWindowTerminal;
}

class QWinMainWindowTerminal : public QMainWindow, public IGui
{
	Q_OBJECT
	
public:
	explicit QWinMainWindowTerminal(QWidget *parent = 0);
	~QWinMainWindowTerminal();
	// ======================================================================
	void init();
	void show();
	
// ======================================================================
	
protected:
	Ui::QWinMainWindowTerminal *ui;
};

#endif // QWINMAINWINDOWTERMINAL_H
