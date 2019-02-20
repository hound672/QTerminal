#include "QMainClass.h"

int main(int argc, char *argv[])
{
	QMainClass::setArgs(argc, argv);
	QMainClass *mainClass = QMainClass::getMainClass();
	int res = mainClass->StartApp();
	delete mainClass;
	return res;
}

// ======================================================================

//	TODO LIST
//	- Разобраться почему программа крешится после закрытия главного окна
//	- Реализовать дамп полученных данных из ком порта
