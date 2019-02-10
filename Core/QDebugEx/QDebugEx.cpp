#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QMutex>
#include <QMutexLocker>

#include "Core/Includes.h"

#include "QDebugEx.h"

QTextStream sTs;
QFile sOutFile;
QMutex sMutex;
QDebugEx::EDebugExFlags sFlags;
bool sDebugMode = false;


// ======================================================================

/**
  * @brief  Конструктор инициализации логирования
  * @param  logName: имя файла для логирования
  * @param  del: флаг удаления файла, если true то файл будет удален перед инициализацией логирования
  * @param  onlyFile: при true пишет лог только в файл
  * @retval None
  */
QDebugEx::QDebugEx(const QString &fileName, EDebugExFlags flags)
{
	sFlags = flags;
	
#ifdef QT_DEBUG
	sDebugMode = true;
#endif
	
	QIODevice::OpenMode mode;
	mode |= QIODevice::WriteOnly;
	mode |= sFlags.testFlag(EDebugExFlag::flgDelOld) ? QIODevice::NotOpen : QIODevice::Append;

	sOutFile.setFileName(fileName);
  if (!sOutFile.open(mode)) {
    throw "Cannot open file!";
  }
  sTs.setDevice(&sOutFile);
	
  qInstallMessageHandler(myMessageOutput);
}

// ======================================================================

void QDebugEx::myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString &msg)
{
  QMutexLocker locker(&sMutex);

  QByteArray localMsg = msg.toLocal8Bit();
  QString str;
  QByteArray typeMessage;
  QString fileName = QString::fromLatin1(context.file);

#ifdef Q_OS_WIN
  QStringList list = fileName.split('\\');
  fileName = list.isEmpty() ? "UNKNOWN_FILE" : list.last();
#endif

	QDateTime dateTime = sFlags.testFlag(EDebugExFlag::flgUseUtc) ? QDateTime::currentDateTimeUtc() :
																																	QDateTime::currentDateTime();
			
	QByteArray date = dateTime.toString("dd.MM.yy hh:mm:ss.zzz").toUtf8();;
	
  switch (type) 
	{
	// ======================================================================  
	case QtDebugMsg:
		
		if (!sDebugMode && !IS_ENV_SET("QDEBUG")) {
			return;
		}
			
		typeMessage = "DBG";
		break;
	// ======================================================================
	case QtWarningMsg:
		typeMessage = "WRN";
		break;
	// ======================================================================
	case QtCriticalMsg:
		typeMessage = "CRI";
		break;
	// ======================================================================
	case QtFatalMsg:
		typeMessage = "FAT";
		break;
	// ======================================================================
	case QtInfoMsg:
		typeMessage = "INF";
		break;
	// ======================================================================
	default:
		typeMessage = "UNK";
	} // switch (type)


  str = QString("[%1] [%2] [%3@%4] %5").arg(date.constData()).arg(typeMessage.constData())
                          .arg(fileName).arg(context.line).arg(localMsg.constData());

  sTs << str << "\r\n";
  sTs.flush();
  if (!sFlags.testFlag(EDebugExFlag::flgOnlyFile)) {
    QTextStream screen(stdout, QIODevice::WriteOnly);
    screen << str << endl;
  }
}

// ======================================================================
