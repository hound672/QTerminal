/**
	* @version  
	* @date 24.01.2019
	* @brief    
	*/
#ifndef QTESTMAINCLASS_H
#define QTESTMAINCLASS_H

#include <QObject>

#include "App/QMainClass.h"

class QTestMainClass : public QObject
{
	Q_OBJECT
	
public:
	explicit QTestMainClass(QObject *parent = 0);
	
// ======================================================================
	
private slots:
	void testSingletone();
	void testTemplateDefines();
	void testEnvSet();
	
// ======================================================================
	
//private:
//	QMainClass *gMainClass;
	
};

#endif // QTESTMAINCLASS_H
