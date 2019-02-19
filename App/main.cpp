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
//  - Вывод данных полученных от ком порта иногда сбиваются, попробовать сделать чтобы данные
//	всегда добавлялись в конец
//	- Блокировать поля для ввода команд при закрытом окне
//	- Разобраться почему программа крешится после закрытия главного окна
//	- Реализовать дамп полученных данных из ком порта
