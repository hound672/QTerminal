/**
  * @version  1.3
  * @brief    Класс для работы с COM портом в отдельном потоке
  */
  #ifndef COMPORTTHREAD_H
#define COMPORTTHREAD_H

#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "Core/Includes.h"
#include "Core/QThreadWorker.h"

// ======================================================================
#define qDebugComPort if (!IS_ENV_SET("QDEBUG_COMPORTTHREAD")); else qDebug
// ======================================================================

class QComPortThread : public QThreadWorker
{
  Q_OBJECT
	
protected:
	enum {
		TIMER_TIMEOUT = 1000,
	};	
	
// ======================================================================
	
public:

	struct SSettings {
		QString mPortName;
		QSerialPort::BaudRate mBoudRate;
		QSerialPort::DataBits mDataBits;
		QSerialPort::Parity mParity;
		QSerialPort::StopBits mStopBits;
		QSerialPort::FlowControl mFlowControl;
	};
	
	static const QMap<QSerialPort::BaudRate, QString> sBaudRateList;
	static const QMap<QSerialPort::DataBits, QString> sDataBitsList;
	static const QMap<QSerialPort::Parity, QString> sParityList;
	static const QMap<QSerialPort::StopBits, QString> sStopBitsList;
	
// ======================================================================

public:
  explicit QComPortThread();
  ~QComPortThread();
	// ======================================================================
	void setSettings(const SSettings &settings);
	void open();
  void close();
  void sendData(const QByteArray &data);
	// ======================================================================
	QList<QSerialPortInfo> getListPorts() const;
	
// ======================================================================

protected:
  QSerialPort *mPort;
	QTimer *mTimer;

// ======================================================================
	
protected:
	bool checkPort() const;
	
// ======================================================================

signals:
	// вызывает после попытки открытия порта
  void signalResultOpen(bool result);
	// вызывается при закрытия порта со стороны системы (например, устройство отключилось и порт пропал)
  void signalClose();
	// вызывается при поступлении новых данных в порт
  void signalIncomingData(const QByteArray &data);
	
// ======================================================================

protected slots:
	void slotStarted();
	void slotStopped();
	void slotTimeout();
	// ======================================================================
	void slotSetSettings(const QComPortThread::SSettings &settings);
  void slotOpen();
  void slotClose();
  void slotSendData(const QByteArray &data);
  void slotReadData();
	
};

#endif // COMPORTTHREAD_H
