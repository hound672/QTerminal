#ifndef QTESTMONITORINGSERVICE_H
#define QTESTMONITORINGSERVICE_H

#include <QObject>

class QTestMonitoringService : public QObject
{
	Q_OBJECT

public:
	explicit QTestMonitoringService(QObject *parent = 0);

// ======================================================================

private slots:
  void testInit();
};

#endif // QTESTMONITORINGSERVICE_H
