//---------------------------------------------------------------------------

#ifndef GameBoardH
#define GameBoardH
//---------------------------------------------------------------------------

#include "Builder.h"

//can flip board
class GameBoard : public Builder {

public:

        GameBoard(SimplyNumbers *simplyGen, Hashtable *movesHash, int gameMode);
        int getCell(int Col, int Row);
        int transform(int x, int y);

protected:
        bool put(TMove N);//can flip board
        int move();


        bool swapX, swapY, swapW;

};

#endif
 