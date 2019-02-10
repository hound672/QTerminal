#include "QTcpServerEx.h"

// ======================================================================

void QTcpServerEx::incomingConnection(qintptr handle)
{
  emit this->SignalNewConnection(handle);
}

// ======================================================================
