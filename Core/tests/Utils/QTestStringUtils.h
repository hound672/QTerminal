/**
  * @version  
  * @brief    Юнит тест для класса QStringUtils
  */
#ifndef QTESTSTRINGUTILS_H
#define QTESTSTRINGUTILS_H

#include <QObject>

class QTestStringUtils : public QObject
{
	Q_OBJECT
public:
	explicit QTestStringUtils(QObject *parent = 0);
	
private slots:
	void testIsValidHex();
	
};

#endif // QTESTSTRINGUTILS_H
