//---------------------------------------------------------------------------
#ifndef positionH
#define positionH
//---------------------------------------------------------------------------
#include "position.h"
#include "tnode.h"
#include "SimplyNumbers.h"
#include "Hashtable.h"

struct PositionHistory
{ unsigned char en[20], nm;
  int symmX, symmY, symmW, symmXYW;
};

class Position {
private:
        SimplyNumbers *simplyGen;
        Hashtable *movesHash;
        PositionHistory history[255];
public:
        Position(SimplyNumbers *simplyGen, Hashtable *movesHash);
        bool Position::forward(int N0, bool& swapX, bool& swapY, bool& swapW);

        TNode *node;
        int count;
};

//---------------------------------------------------------------------------
#endif