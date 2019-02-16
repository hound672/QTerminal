/**
  * @version  
  * @brief    Класс для работы с машиной состояний
  */
#ifndef QSTATEMACHINEEX_H
#define QSTATEMACHINEEX_H

#include <QObject>
#include <QTimer>
#include <QMap>

#include "Core/Includes.h"

class QStateMachineEx : public QObject
{
	Q_OBJECT
	
public:
	typedef QMap<int, QString> TStateName;
	
	enum {
		_stStartup = 0,
	};
	
// ======================================================================

public:
	explicit QStateMachineEx(QObject *parent = 0);
	explicit QStateMachineEx(const QString &className, QObject *parent = 0);
	explicit QStateMachineEx(const QString &className, const TStateName &stateNames, QObject *parent = 0);

// ======================================================================	
	
protected:
	void setTimer(quint32 timeout = U32_MAX_VALUE);
	void setState(int newState);
	int getState() const;
	QString getStateStr() const;
	// ======================================================================
	virtual void onEvTimer() {}
	
// ======================================================================
	
private:
	int mState;
	QTimer mTimer;
	QString mWorkerName;
	TStateName mStateNames;
	
// ======================================================================
	
private slots:
	void slotTimer();
	
};

#endif // QSTATEMACHINEEX_H
