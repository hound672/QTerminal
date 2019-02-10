/**
  * @version
  * @brief    Обертка поверх класса QAmqp
  */
#ifndef QMQCLIENT_H
#define QMQCLIENT_H

#ifdef USE_RABBIT_MQ

#include <QObject>
#include <QEventLoop>
#include <QTimer>

#include "qamqpclient.h"
#include "qamqpexchange.h"
#include "qamqpqueue.h"
#include "qamqpmessage.h"

#include "Core/Includes.h"
#include "Core/QSettingsApp.h"


// ======================================================================
#define qMqClientDebug if (!IS_ENV_SET("QMQCLIENT_DEBUG")); else qDebug
// ======================================================================


class QMqClient : public QAmqpClient
{
  Q_OBJECT

// ======================================================================

public:
  explicit QMqClient(const QSettingsApp::SRABBIT_MQ &settings, QObject *parent = 0);
	// ======================================================================
  bool ConnectToServer();

  // exchanges
  QAmqpExchange* DeclareExchange(const QString &name = QString(),
                       QAmqpExchange::ExchangeType type = QAmqpExchange::Direct,
                       QAmqpExchange::ExchangeOptions options = QAmqpExchange::NoOptions);

  // queue
  QAmqpQueue* DeclareQueue(const QString &name = QString(),
                            int options = QAmqpQueue::Durable | QAmqpQueue::AutoDelete);
  bool Bind(QAmqpQueue *queue, QAmqpExchange *exchange,
                    const QString &routingKey = QString());
  bool Bind(QAmqpQueue *queue, const QString &exchangeName,
                    const QString &routingKey = QString());
  bool WaitMsg(QAmqpQueue *queue, int timeout, const QString &correlationId, QAmqpMessage &message);

// ======================================================================
	
private:
  QEventLoop mLoop;
	QSettingsApp::SRABBIT_MQ mSettings;

};

#endif // USE_RABBIT_MQ

#endif // QMQCLIENT_H
