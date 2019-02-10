#include <QTest>

#include "Core/Utils/QStringUtils.h"

#include "QTestStringUtils.h"

// ======================================================================

QTestStringUtils::QTestStringUtils(QObject *parent) : QObject(parent)
{
	
}

// ======================================================================

// ======================================================================
//  private slots                       
// ======================================================================

/**
	* @brief  Проверка метода провки строки на HEX символы
	* @param  
	* @retval 
	*/
void QTestStringUtils::testIsValidHex()
{
	QByteArray validHex1("AB4287AEFBC458213");
	QByteArray validHex2("abef45421fba");
	QByteArray validHex3("fB4287deaBC458213");
	QByteArray invalidHex1("AB4287AEFBCH458213");
	QByteArray invalidHex2("abef45421grtfba");
	QByteArray invalidHex3("fVB42f87deaGBC458k213");
	
	QCOMPARE(QStringUtils::isValidHex(validHex1), true);
	QCOMPARE(QStringUtils::isValidHex(validHex2), true);
	QCOMPARE(QStringUtils::isValidHex(validHex3), true);
	QCOMPARE(QStringUtils::isValidHex(invalidHex1), false);
	QCOMPARE(QStringUtils::isValidHex(invalidHex2), false);
	QCOMPARE(QStringUtils::isValidHex(invalidHex3), false);
}

// ======================================================================
