#ifndef QMAINCLASS_H
#define QMAINCLASS_H

#include <QObject>

#include "Core/QSetupClass.h"

#include "Gui/Win/QWinMainWindowTerminal.h"

class QMainClass : public QSetupClass
{
	Q_OBJECT

public:
	explicit QMainClass(int argc, char *argv[]);
	~QMainClass();
	
// ======================================================================
	
protected:
	void makeSignalSlots();
	int setup();
	int start();
	
// ======================================================================
	
protected:
	QWinMainWindowTerminal *mMainWindow;	
	
};

#endif // QMAINCLASS_H
