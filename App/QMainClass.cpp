#include <QDebug>

#include "QMainClass.h"

// ======================================================================
static int sArgc;
static char **sArgv;
// ======================================================================

QMainClass::QMainClass(int argc, char *argv[]) : 
	QSetupClass(argc, argv)
{
}

// ======================================================================

QMainClass::~QMainClass()
{
	delete mGui;	
	delete mTerminal;
	mPort->StopWorker();
	delete mPort;
}

// ======================================================================

/**
	* @brief  Сохраняет переданые при запуске программы аргументы
	* @param  
	* @retval 
	*/
void QMainClass::setArgs(int argc, char **argv)
{
	sArgc = argc;
	sArgv = argv;
}

QMainClass *QMainClass::getMainClass()
{
	static QMainClass mainClass(sArgc, sArgv);
	return &mainClass;
}

// ======================================================================

// ======================================================================
//  protected                       
// ======================================================================

/**
	* @brief  Создает связку сигналов-слотов
	* @param  
	* @retval 
	*/
void QMainClass::makeSignalSlots()
{
}

// ======================================================================

/**
	* @brief  Инициализация компонентов для запуска приложения
	* @param  
	* @retval 
	*/
int QMainClass::setup()
{
	mGui = new QWinMainWindowTerminal();
	mTerminal = new QTerminal();
	mPort = new QComPortThread();

	mGui->init();
	mTerminal->init();
	
	return 1;
}

// ======================================================================

/**
	* @brief  Запуск приложения
	* @param  
	* @retval 
	*/
int QMainClass::start()
{
	mPort->StartWorker();
	mGui->show();
	return 1;
}

// ======================================================================
