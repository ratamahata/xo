//---------------------------------------------------------------------------
#ifndef positionH
#define positionH
//---------------------------------------------------------------------------

#include "tnode.h"
#include "SimplyNumbers.h"
#include "Hashtable.h"

#define fsize 15 //field size

struct PositionHistory
{ unsigned char en[16], nm;
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

        unsigned char kl[fsize*fsize], // 0 == empty, disabled
                                       // 1 == empty, enabled
                id;                       // 0 == 'x',first

};

//---------------------------------------------------------------------------
#endif