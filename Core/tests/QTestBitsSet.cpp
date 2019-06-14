#include <QTest>
#include "QTestBitsSet.h"

#include "../Utils/QBitsSet.h"

// ======================================================================

QTestBitsSet::QTestBitsSet(QObject *parent) : QObject(parent)
{
	
}

// ======================================================================

void QTestBitsSet::testGetBitsSet()
{
	QBitsSet bitSet(0x41); // position is 6
	QCOMPARE(bitSet.getBitsSet(), (quint32)0x41);
}

// ======================================================================

void QTestBitsSet::testSetBit()
{
	QBitsSet bitSet; // position is 6
	
	QCOMPARE(bitSet.testBit(6), false);
	bitSet.setBit(6);
	QCOMPARE(bitSet.testBit(6), true);
}

// ======================================================================

void QTestBitsSet::testTestBit()
{
	QBitsSet bitSet(0x41); // position is 6
	
	QCOMPARE(bitSet.testBit(6), true);
}

// ======================================================================

void QTestBitsSet::testResetBit()
{
	QBitsSet bitSet(0x42); // position is 6
	
	QCOMPARE(bitSet.testBit(6), true);
	QCOMPARE(bitSet.testBit(1), true);
	bitSet.resetBit(6);
	QCOMPARE(bitSet.testBit(6), false);
	QCOMPARE(bitSet.testBit(1), true);
}

// ======================================================================

void QTestBitsSet::testCopyObject()
{
	QBitsSet bitSet1(0x40); // position is 6
	QBitsSet bitSet2;
	
	QCOMPARE(bitSet2.testBit(6), false);
	bitSet2 = bitSet1;
	QCOMPARE(bitSet2.testBit(6), true);
}

// ======================================================================

void QTestBitsSet::testIsEmpty()
{
	QBitsSet bitSet(0x40); // position is 6
	
	QCOMPARE(bitSet.isEmpty(), false);
	bitSet.resetBit(6);
	QCOMPARE(bitSet.isEmpty(), true);
}

// ======================================================================

void QTestBitsSet::testTestBits()
{
	QBitsSet bitSet(0x123);
	QCOMPARE(bitSet.testBits(0x123), true);
}

// ======================================================================
