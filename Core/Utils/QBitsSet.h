/**
	* @version  
	* @brief    Class for workflow with bits set
	*/
#ifndef QBITSSET_H
#define QBITSSET_H

#include <Qt>

class QBitsSet
{
public:
	QBitsSet(quint32 initialState = 0);
	inline QBitsSet& operator = (const QBitsSet& set) { mBits = set.mBits; return *this; }
	// ======================================================================
	quint32 getBitsSet() const {return mBits;}
	void setBit(quint8 pos);
	void resetBit(quint8 pos);
	bool testBit(quint8 pos) const;
	bool testBits(quint32 mask) const;
	bool isEmpty() const;
	
// ======================================================================
	
protected:
	quint32 mBits;
};

#endif // QBITSSET_H
