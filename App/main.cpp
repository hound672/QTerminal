#include "AppConfig.h"
#include "QMainClass.h"

// ======================================================================
QMainClass *gMainClass;
// ======================================================================

int main(int argc, char *argv[])
{
	gMainClass = new QMainClass(argc, argv);
	int res = gMainClass->StartApp();
	delete gMainClass;
	return res;
}

// ======================================================================

//	TODO LIST
