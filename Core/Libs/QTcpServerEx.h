/**
  * @version
  * @brief    Класс обертка над классом QTcpServer
  */
#ifndef QTCPSERVEREX_H
#define QTCPSERVEREX_H

#include <QObject>
#include <QTcpServer>

class QTcpServerEx : public QTcpServer
{
  Q_OBJECT
public:
  explicit QTcpServerEx(QObject *parent = 0) : QTcpServer(parent) {}
  void incomingConnection(qintptr handle);

// ======================================================================
	
signals:
  void SignalNewConnection(quintptr handle);

};

#endif // QTCPSERVEREX_H
