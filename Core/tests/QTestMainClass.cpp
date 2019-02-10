#include <QDebug>
#include <QTest>

#include "QTestMainClass.h"
#include "Core/Includes.h"
#include "App/AppConfig.h"

// ======================================================================
QMainClass *gMainClass;
// ======================================================================

// ======================================================================

QTestMainClass::QTestMainClass(QObject *parent) : QObject(parent)
{
	static const char *argv[] = {
		"APP.EXE",
		"--env",
		"SOME_ENV",
		"-e",
		"NEW_ENV=CHECK"
	};
	QMainClass::setArgs(_DIM(argv), (char**)argv);
	gMainClass = QMainClass::getMainClass();
}

// ======================================================================

// ======================================================================
//  private slots                       
// ======================================================================

/**
	* @brief  Тест: объект возвращаемый статическим методом QMainClass::getMainClass является синглтоном
	* @param  
	* @retval 
	*/
void QTestMainClass::testSingletone()
{
	QCOMPARE(gMainClass, QMainClass::getMainClass());
}

// ======================================================================

/**
	* @brief  Тест: дефайны прилоежния прописанные в pro файле
	* @param  
	* @retval 
	*/
void QTestMainClass::testTemplateDefines()
{
	QCOMPARE(gMainClass->getAppName(), QString("QT_BLANK"));
	QCOMPARE(gMainClass->getAppVersion(), QString("0.0.0.1"));
	QCOMPARE(gMainClass->getSettingsFileName(), QString("settings.ini"));
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
