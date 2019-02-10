#include <QCoreApplication>
#include <QTest>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "QTestMonitoringService.h"
#include "QTestMainClass.h"

// Libs
#include "Libs/QTestComPortThread.h"

// Utils
#include "Utils/QTestStringUtils.h"

int main(int argc, char *argv[])
{
//  std::freopen("TestsCore_UnitTests.log", "w", stdout);
	
	QTest::qExec(new QTestMonitoringService, argc, argv);
	QTest::qExec(new QTestMainClass, argc, argv);
//	QTest::qExec(new QTestComPortThread, argc, argv);
	QTest::qExec(new QTestStringUtils, argc, argv);
  
  return 0;
}
