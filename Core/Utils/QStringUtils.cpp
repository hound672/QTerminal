#include <QRegularExpression>

#include "QStringUtils.h"

// ======================================================================

QStringUtils::QStringUtils()
{
}

// ======================================================================

/**
	* @brief  Проверяет являются ли переданные символы валидными HEX значениями
	* @param  data: данные для проверки
	* @retval true: если строка содержит только HEX символы
	*/
bool QStringUtils::isValidHex(const QByteArray &data)
{
	QRegularExpression hexMatcher("^[0-9A-F]+$",
																QRegularExpression::CaseInsensitiveOption);
	
	QRegularExpressionMatch match = hexMatcher.match(data);
	return match.hasMatch();
}

// ======================================================================
