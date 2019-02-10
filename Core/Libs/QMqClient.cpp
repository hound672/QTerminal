#ifdef USE_RABBIT_MQ

#include <QDebug>
#include <QTimer>

#include "QMqClient.h"

QMqClient::QMqClient(const QSettingsApp::SRABBIT_MQ &settings, QObject *parent) :
  QAmqpClient(parent),
  mSettings(settings)
{
  connect(this, &QMqClient::connected,
          &mLoop, &QEventLoop::quit);
  connect(this, SIGNAL(socketError(QAbstractSocket::SocketError)),
          &mLoop, SLOT(quit()));
}

// ======================================================================

// ======================================================================
//  public
// ======================================================================

/**
  * @brief  Подключаемся к серверу
  * @param
  * @retval true: в случае успешного подключения
  */
bool QMqClient::ConnectToServer()
{
  qMqClientDebug() << "Start connect to RabbitMQ."
                   << "\n HOST: " << mSettings.HOST
                   << "\n PORT: " << mSettings.PORT
                   << "\n VIRTUAL HOST: " << mSettings.VIRTUAL_HOST
                   << "\n USER: " << mSettings.USER
                   << "\n PASS: " << mSettings.PASS;

  QAmqpClient::setHost(mSettings.HOST);
  QAmqpClient::setPort(mSettings.PORT);
  QAmqpClient::setVirtualHost(mSettings.VIRTUAL_HOST);
  QAmqpClient::setUsername(mSettings.USER);
  QAmqpClient::setPassword(mSettings.PASS);

  QAmqpClient::connectToHost();
  mLoop.exec();

  return QAmqpClient::isConnected();
}

// ======================================================================

/**
  * @brief  Объяевляет точку обмена
  * @param
  * @retval
  */
QAmqpExchange *QMqClient::DeclareExchange(const QString &name,
                                QAmqpExchange::ExchangeType type,
                                QAmqpExchange::ExchangeOptions options)
{
  qMqClientDebug() << "Start declare exchange: " << name;
  QAmqpExchange *exchange = QAmqpClient::createExchange(name);
  if (name.isEmpty()) {
    return exchange;
  }

  connect(exchange, SIGNAL(declared()),
          &mLoop, SLOT(quit()));
  connect(exchange, SIGNAL(error(QAMQP::Error)),
          &mLoop, SLOT(quit()));
  connect(exchange, SIGNAL(allMessagesDelivered()),
          &mLoop, SLOT(quit()));

  exchange->declare(type, options);
  mLoop.exec();

  return exchange;
}

// ======================================================================

/**
  * @brief  Обявляем очередь
  * @param
  * @retval
  */
QAmqpQueue *QMqClient::DeclareQueue(const QString &name, int options)
{
  qMqClientDebug() << "Start declare queue";
  QAmqpQueue *queue = QAmqpClient::createQueue(name);

  if (queue->isDeclared()) {
    return queue;
  }

  connect(queue, SIGNAL(declared()),
          &mLoop, SLOT(quit()));
  connect(queue, SIGNAL(error(QAMQP::Error)),
          &mLoop, SLOT(quit()));
  connect(queue, &QAmqpQueue::bound,
          &mLoop, &QEventLoop::quit);
  connect(queue, SIGNAL(messageReceived()),
          &mLoop, SLOT(quit()));

  queue->declare(options);
  mLoop.exec();

  return queue;
}

// ======================================================================

/**
  * @brief  Биндим очередь к точке обмена
  * @param
  * @retval
  */
bool QMqClient::Bind(QAmqpQueue *queue, QAmqpExchange *exchange, const QString &routingKey)
{
  return this->Bind(queue, exchange->name(), routingKey);
}

// ======================================================================

bool QMqClient::Bind(QAmqpQueue *queue, const QString &exchangeName, const QString &routingKey)
{
  queue->bind(exchangeName, routingKey);
  mLoop.exec();

  return queue->error() == QAMQP::NoError;
}

// ======================================================================

/**
  * @brief  Дожидаемся ответа из очереди
  * @param
  * @retval
  */
bool QMqClient::WaitMsg(QAmqpQueue *queue, int timeout, const QString &correlationId, QAmqpMessage &message)
{
  QTimer::singleShot(timeout, &mLoop, SLOT(quit()));
  QAmqpMessage msg;

  while (true) {
    mLoop.exec();
    if (queue->isEmpty()) {
      // event завершился по таймауту, но сообщений нет - выходим полностью с ошибкой
      return false;
    }

    // сообщение есть, проверяем CorrelationId
    msg = queue->dequeue();
    if (msg.property(QAmqpMessage::CorrelationId).toString() == correlationId) {
      // correleactionId совпал
      message = msg;
      return true;
    }

  } // while (true)

  return false;
}

// ======================================================================

#endif // USE_RABBIT_MQ
