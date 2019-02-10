#include <QTest>
#include <QDebug>

#include "QTestMonitoringService.h"

// ======================================================================

QTestMonitoringService::QTestMonitoringService(QObject *parent): 
	QObject(parent)
{
  
}

// ======================================================================

// ======================================================================
//  private slots                       
// ======================================================================

/**
  * @brief  Тест для инициализации сервиса
  * @param  
  * @retval 
  */
void QTestMonitoringService::testInit()
{
  QCOMPARE(1, 1);  
}

// ======================================================================
