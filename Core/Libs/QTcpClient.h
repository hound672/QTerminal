/**
  * @version
  * @brief    Абстрактный класс для работы с Tcp клиентами
  */
#ifndef QCLIENT_H
#define QCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QElapsedTimer>

class QTcpClient : public QObject
{
  Q_OBJECT
	
public:
  explicit QTcpClient(QObject *parent = 0);
  ~QTcpClient();
	// ======================================================================
	quint64 GetLifeTime() const {return mLifeTime.elapsed();}
  quintptr GetConnectionId() const {return mSocketId;}
  virtual void SetConnectionId(qintptr socketId);
  virtual qint64 SendData(const QByteArray &dataToSend);
  virtual void CloseSocket();

// ======================================================================
	
protected:
	QElapsedTimer mLifeTime;
  QTcpSocket *mSocket;
  quintptr mSocketId;
	
// ======================================================================

signals:
  void SignalDisconnected();
	
// ======================================================================

protected slots:
	virtual void SlotReadData() {}
  virtual void SlotDisconnected();
};

#endif // QCLIENT_H
