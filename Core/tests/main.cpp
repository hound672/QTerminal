#include <QCoreApplication>
#include <QTest>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "QTestMonitoringService.h"
#include "QTestMainClass.h"
#include <QDebug>
int main(int argc, char *argv[])
{
//  std::freopen("TestsCore_UnitTests.log", "w", stdout);
	
	QTest::qExec(new QTestMonitoringService, argc, argv);
	QTest::qExec(new QTestMainClass, argc, argv);
  
  return 0;
}
