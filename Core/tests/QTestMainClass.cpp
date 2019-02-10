#include <QTest>

#include "QTestMainClass.h"
#include "Core/Includes.h"

// ======================================================================
#include <QDebug>
QTestMainClass::QTestMainClass(QObject *parent) : QObject(parent)
{
	static const char *argv[] = {
		"APP.EXE",
		"--env",
		"SOME_ENV",
		"-e",
		"NEW_ENV=CHECK"
	};
	mMainClass = new QMainClass(_DIM(argv), (char**)argv);
}

// ======================================================================

// ======================================================================
//  private slots                       
// ======================================================================

/**
	* @brief  Тест: дефайны прилоежния прописанные в pro файле
	* @param  
	* @retval 
	*/
void QTestMainClass::testTemplateDefines()
{
	QCOMPARE(mMainClass->getAppName(), QString("QT_BLANK"));
	QCOMPARE(mMainClass->getAppVersion(), QString("0.0.0.1"));
	QCOMPARE(mMainClass->getSettingsFileName(), QString("settings.ini"));
}

// ======================================================================

/**
	* @brief  Тест: проверка на выставление переменой окружения
	* @param  
	* @retval 
	*/
void QTestMainClass::testEnvSet()
{
	QCOMPARE(IS_ENV_SET("SOME_ENV"), true);	
	QCOMPARE(IS_ENV_SET("--env"), false);	
	QCOMPARE(IS_ENV_SET("-e"), false);	
	QCOMPARE(IS_ENV_CMP("NEW_ENV", "CHECK"), true);	
}

// ======================================================================
