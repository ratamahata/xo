//---------------------------------------------------------------------------

#ifndef GameBoardH
#define GameBoardH
//---------------------------------------------------------------------------

#include "Builder.h"

//can flip board
class GameBoard : public Builder {

public:
        bool *swapX, *swapY, *swapW, *swapXYW;
        Logger *logger;

        GameBoard(SimplyNumbers *simplyGen, Hashtable *movesHash, Logger *logger,
                 bool *swapX, bool *swapY, bool *swapW, bool *swapXYW, int gameMode);

        bool put(TMove N);//can flip board
        int move();

};

#endif
 