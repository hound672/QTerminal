#include <QDebug>
#include <QThread>

#include "QTcpClient.h"

// ======================================================================

QTcpClient::QTcpClient(QObject *parent) : QObject(parent)
{
	mLifeTime.start();
  mSocket = new QTcpSocket();

  connect(mSocket, &QTcpSocket::readyRead,
          this, &QTcpClient::SlotReadData, Qt::QueuedConnection);
  connect(mSocket, &QTcpSocket::disconnected,
          this, &QTcpClient::SlotDisconnected, Qt::QueuedConnection);
}

// ======================================================================

QTcpClient::~QTcpClient()
{
  delete mSocket;
}

// ======================================================================

// ======================================================================
//  public
// ======================================================================

/**
  * @brief  Изменяет для данного TCP клиента его дескриптор
  * @param  socketDescriptor: новый дескриптор
  * @retval
  */
void QTcpClient::SetConnectionId(qintptr socketId)
{
	mLifeTime.start();
  mSocketId = socketId;
  mSocket->setSocketDescriptor(socketId);
}

// ======================================================================

/**
  * @brief  Отправляет данные Tcp клиенту
  * @param  dataToSend: данные для отправки
  * @retval
  */
qint64 QTcpClient::SendData(const QByteArray &dataToSend)
{
  return mSocket->write(dataToSend);
}

// ======================================================================

/**
  * @brief  Закрывает соединения для текущего клиента
  * @param
  * @retval
  */
void QTcpClient::CloseSocket()
{
  mSocket->close();
}

// ======================================================================

// ======================================================================
//  protected slots
// ======================================================================

void QTcpClient::SlotDisconnected()
{
  // "костыль" иначе валгринд ругается, что при самостоятельном отключении клиента теряется память
  mSocket->deleteLater();
  mSocket = NULL;
  emit SignalDisconnected();
}

// ======================================================================
