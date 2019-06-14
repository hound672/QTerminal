#include <QDebug>
#include <QEventLoop>
#include <QNetworkReply>

#include "QServerApi.h"

// ======================================================================
const QServerApi::TRequestSenders QServerApi::sRequestSenders = {
  {QServerApi::requestGet, &QServerApi::sendGet},
  {QServerApi::requestPost, &QServerApi::sendPost},
  {QServerApi::requestPut, &QServerApi::sendPut},
};

// ======================================================================

QServerApi::QServerApi(QObject *parent):
  QObject(parent)
{
//  mRequest = new QNetworkRequest();
//  mManager = new QNetworkAccessManager(this);
}

QServerApi::~QServerApi()
{
//  delete mRequest;
}

// ======================================================================

/**
  * @brief  Send request to server
  * @param
  * @retval
  */
QByteArray QServerApi::sendRequest(const QString &url, int &resultCode,
                                   ERequestTypes requestType,
                                   const QByteArray &dataToSend)
{
	QUrl link(url);
	return sendRequest(link, resultCode, requestType, dataToSend);
}

QByteArray QServerApi::sendRequest(const QUrl &url, int &resultCode, 
																	 QServerApi::ERequestTypes requestType, const QByteArray &dataToSend)
{
	qDebug() << "Start send request. Url: " << url;
  mRequest.setUrl(url);

  QNetworkReply *reply;

  if (!sRequestSenders[requestType]) {
    return QByteArray();
  }

  reply = (this->*sRequestSenders[requestType])(&mRequest, dataToSend);

  QEventLoop eventLoop;
  QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
  eventLoop.exec();

  resultCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  QByteArray readData = reply->readAll();
  reply->deleteLater();
  delete reply;
	return readData;
}

QJsonDocument QServerApi::sendRequest(const QUrl &url, int &resultCode, 
																	 QServerApi::ERequestTypes requestType, const QJsonObject &dataToSend)
{
	QByteArray res = this->sendRequest(url, resultCode, requestType, QJsonDocument(dataToSend).toJson());
	return QJsonDocument::fromJson(res);
}

// ======================================================================

// ======================================================================
//  request sender
// ======================================================================

/**
  * @brief  Отправка GET запроса
  * @param
  * @retval
  */
QNetworkReply *QServerApi::sendGet(QNetworkRequest *request, const QByteArray &)
{
  return mManager.get(*request);
}

// ======================================================================

/**
  * @brief  Оптправка POST запроса
  * @param
  * @retval
  */
QNetworkReply *QServerApi::sendPost(QNetworkRequest *request, const QByteArray &data)
{
  return mManager.post(*request, data);
}

// ======================================================================

/**
  * @brief  Оптправка PUT запроса
  * @param
  * @retval
  */
QNetworkReply *QServerApi::sendPut(QNetworkRequest *request, const QByteArray &data)
{
  return mManager.put(*request, data);
}

// ======================================================================
