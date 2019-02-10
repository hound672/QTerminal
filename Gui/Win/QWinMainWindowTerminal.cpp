#include <QDebug>

#include "QWinMainWindowTerminal.h"
#include "ui_QWinMainWindowTerminal.h"

// ======================================================================

QWinMainWindowTerminal::QWinMainWindowTerminal(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::QWinMainWindowTerminal)
{
	ui->setupUi(this);
}

// ======================================================================

QWinMainWindowTerminal::~QWinMainWindowTerminal()
{
	delete ui;
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
