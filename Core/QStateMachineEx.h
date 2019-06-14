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

class QStateMachineEx
{
public:
	typedef QMap<int, QString> TStateName;
	
	enum {
		_stStartup = 0,
	};
	
// ======================================================================

public:
	explicit QStateMachineEx();
	explicit QStateMachineEx(const QString &className);
	explicit QStateMachineEx(const QString &className, const TStateName &stateNames);

// ======================================================================	
	
protected:
	void setState(int newState);
	int getState() const;
	QString getStateStr() const;
	// ======================================================================
	virtual void onEvTimer() {}
	
// ======================================================================
	
private:
	int mState;
	QString mWorkerName;
	TStateName mStateNames;
	
// ======================================================================
	
};

#endif // QSTATEMACHINEEX_H
