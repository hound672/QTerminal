/**
  * @version  
  * @brief    Набор утилит для работы со строками
  */
#ifndef QSTRINGUTILS_H
#define QSTRINGUTILS_H

#include <QByteArray>

class QStringUtils
{
	
public:
	QStringUtils();
	// ======================================================================
	static bool isValidHex(const QByteArray &data);
	
// ======================================================================
	
};

#endif // QSTRINGUTILS_H
