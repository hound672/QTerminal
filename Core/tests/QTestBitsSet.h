#ifndef QTESTBITSSET_H
#define QTESTBITSSET_H

#include <QObject>

class QTestBitsSet : public QObject
{
	Q_OBJECT

public:
	explicit QTestBitsSet(QObject *parent = 0);

// ======================================================================
	
private slots:
	void testGetBitsSet();
	void testSetBit();
	void testTestBit();
	void testResetBit();
	void testCopyObject();
	void testIsEmpty();
	void testTestBits();
	
};

#endif // QTESTBITSSET_H
