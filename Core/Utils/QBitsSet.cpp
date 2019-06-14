#include "QBitsSet.h"

// ======================================================================

QBitsSet::QBitsSet(quint32 initialState):
	mBits(initialState)
{
	
}

// ======================================================================

// ======================================================================
//  public                       
// ======================================================================

/**
	* @brief  Set bit's state 
	* @param  pos: bit's position
	* @retval void
	*/
void QBitsSet::setBit(quint8 pos)
{
	mBits |= 1 << (pos & 31);
}

// ======================================================================

/**
	* @brief  Reset bit's state
	* @param  pos: bit's position
	* @retval void
	*/
void QBitsSet::resetBit(quint8 pos)
{
	mBits &= ~(1 << (pos & 31));
}

// ======================================================================

/**
	* @brief  Check if bit is set
	* @param  pos: bit's position
	* @retval true if bit is set
	*/
bool QBitsSet::testBit(quint8 pos) const
{
	return mBits & (1 << (pos & 31));
}

// ======================================================================

/**
	* @brief  Test bit mask
	* @param  
	* @retval 
	*/
bool QBitsSet::testBits(quint32 mask) const
{
	return mBits & mask;
}

// ======================================================================

/**
	* @brief  Check if bits set is empty
	* @param  
	* @retval true if bits set is empty
	*/
bool QBitsSet::isEmpty() const
{
	return mBits == 0;
}

// ======================================================================
