#ifndef QMAINCLASS_H
#define QMAINCLASS_H

#include <QObject>

#include "Core/QSetupClass.h"

#include "Collections/Interfaces.h"
#include "Gui/Win/QWinMainWindowTerminal.h"
#include "Core/Libs/QComPortThread.h"
#include "Terminal/QTerminal.h"

class QMainClass : public QSetupClass
{
	Q_OBJECT

private:
	explicit QMainClass(int argc, char *argv[]);
	
// ======================================================================
	
public:
	~QMainClass();
	static void setArgs(int argc, char **argv);
	static QMainClass *getMainClass();
	// ======================================================================
	IGui *getGui() {return mGui;}
	QTerminal *getTerminal() {return mTerminal;}
	QComPortThread *getPort() {return mPort;}
	
// ======================================================================
	
protected:
	void makeSignalSlots();
	int setup();
	int start();
	
// ======================================================================
	
protected:
	IGui *mGui;	
	QTerminal *mTerminal;
	QComPortThread *mPort;
	
};

#endif // QMAINCLASS_H
