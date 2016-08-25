//---------------------------------------------------------------------------

#ifndef simplynumbersH
#define simplynumbersH

#include "tnode.h"
//---------------------------------------------------------------------------

class SimplyNumbers {
public:
  unsigned long simplyNumbers[225]; //ordinal->simplyNumber
  unsigned long hashValues[225]; // move->simplyNumber
  int simplyCounter;
  unsigned long getHash(TMove move);

  SimplyNumbers();
};

#endif
