#ifndef QMAINCLASS_H
#define QMAINCLASS_H

#include <QObject>

#include "Core/QSetupClass.h"

class QMainClass : public QSetupClass
{
	Q_OBJECT

public:
	explicit QMainClass(int argc, char *argv[]);
	
// ======================================================================
	
protected:
	void makeSignalSlots();
	int setup();
	int start();
	
// ======================================================================
	
};

#endif // QMAINCLASS_H
