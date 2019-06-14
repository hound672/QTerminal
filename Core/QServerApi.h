/**
  * @version
  * @brief    Класс для работы с http запросами (Api)
  */
#ifndef SERVERAPI_H
#define SERVERAPI_H

#include <QObject>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

class QServerApi : public QObject
{
  Q_OBJECT

public:
  enum EHttpCodes {
    httpCantReached = 0,
    httpSuccess = 200,
    httpCreated = 201,
    httpErrorAnswer = 400,
    httpErrorAuth = 401,
    httpNotFound = 404,
    httpErrorServer = 500,
  };

  enum ERequestTypes {
    requestGet = 0,
    requestPost,
    requestPut,
    requestDelete
  };
	
// ======================================================================

public:
  explicit QServerApi(QObject *parent = 0);
  ~QServerApi();
	// ======================================================================
	QByteArray sendRequest(const QString &url, int &resultCode,
                         ERequestTypes requestType = requestGet,
                         const QByteArray &dataToSend = QByteArray());
  QByteArray sendRequest(const QUrl &url, int &resultCode,
                         ERequestTypes requestType = requestGet,
                         const QByteArray &dataToSend = QByteArray());
	QJsonDocument sendRequest(const QUrl &url, int &resultCode,
                         ERequestTypes requestType = requestGet,
                         const QJsonObject &dataToSend = QJsonObject());
	
// ======================================================================
	
protected:
  typedef QNetworkReply* (QServerApi::* TRequestSender)(QNetworkRequest*, const QByteArray&);
  typedef QHash<QServerApi::ERequestTypes, TRequestSender> TRequestSenders;
  static const TRequestSenders sRequestSenders;
	
// ======================================================================

protected:
  QNetworkRequest mRequest;
  QNetworkAccessManager mManager;
	
// ======================================================================

protected:
  // request sender
  QNetworkReply* sendGet(QNetworkRequest *request, const QByteArray&);
  QNetworkReply* sendPost(QNetworkRequest *request, const QByteArray &data);
  QNetworkReply* sendPut(QNetworkRequest *request, const QByteArray &data);
	
};

#endif // SERVERAPI_H
