#include <QDebug>

#include "QStateMachineEx.h"


// ======================================================================

QStateMachineEx::QStateMachineEx():
	QStateMachineEx(QString(), TStateName())
{
}

QStateMachineEx::QStateMachineEx(const QString &className):
	QStateMachineEx(className, TStateName())	
{
}

QStateMachineEx::QStateMachineEx(const QString &className, const TStateName &stateNames) : 
	mState(_stStartup),
	mWorkerName(className),
	mStateNames(stateNames)
{
}

// ======================================================================

// ======================================================================
//  protected                       
// ======================================================================

/**
	* @brief  Смена состояния
	* @param  
	* @retval 
	*/
void QStateMachineEx::setState(int newState)
{
	qInfo()	<< "[" << mWorkerName << "]"
					<< "************* Set state from: " << mStateNames.value(mState, QString::number(mState))
					<< " to: " << mStateNames.value(newState, QString::number(newState))
					<< " ******************";
	mState = newState;
}

// ======================================================================

/**
	* @brief  Возвращает текущее состояние
	* @param  
	* @retval 
	*/
int QStateMachineEx::getState() const
{
	return mState;
}

// ======================================================================

/**
	* @brief  Возвращает строковое представление состояния
	* @param  
	* @retval 
	*/
QString QStateMachineEx::getStateStr() const
{
	return mStateNames.value(getState());
}

// ======================================================================
