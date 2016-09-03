//---------------------------------------------------------------------------

#ifndef GameBoardH
#define GameBoardH
//---------------------------------------------------------------------------

#include "Builder.h"

//can flip board
class GameBoard : public Builder {

public:
        bool *swapX, *swapY, *swapW;

        GameBoard(SimplyNumbers *simplyGen, Hashtable *movesHash,
                 bool *swapX, bool *swapY, bool *swapW);

        bool put(TMove N);//can flip board

};

#endif
 