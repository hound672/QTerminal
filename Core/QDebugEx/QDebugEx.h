/**
  * @version  1.12
  * @brief    Замена стандартного логера
  */
#ifndef QDEBUGEX_H
#define QDEBUGEX_H

#include <QString>
#include <QFlags>

class QDebugEx
{
	
public:
	enum EDebugExFlag {
		flgNoOption			= 0x00,
		flgDelOld				= 0x01,
		flgOnlyFile			= 0x02,
		flgUseUtc				= 0x04,
	};
	Q_DECLARE_FLAGS(EDebugExFlags, EDebugExFlag)
	
// ======================================================================
	
public:
  QDebugEx(const QString &fileName, EDebugExFlags flags = flgNoOption);

// ======================================================================
	
private:
  static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

// ======================================================================

};

Q_DECLARE_OPERATORS_FOR_FLAGS(QDebugEx::EDebugExFlags)

#endif // QDEBUGEX_H
