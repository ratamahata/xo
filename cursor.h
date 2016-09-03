//---------------------------------------------------------------------------

#ifndef cursorH
#define cursorH
#include "tnode.h"
#include "SimplyNumbers.h"
#include "Hashtable.h"

#define fsize 15 //field size
//---------------------------------------------------------------------------

struct CursorHistory {
  CursorHistory();
  TMove en[16],//"Enabled" moves buffer
        move;
  int enCount;//count of actually "Enabled" moves
  int symmX, symmY, symmXY, symmW, symmXW, symmYW, symmXYW;
  TNode *node;
};

class Cursor {
private:
protected:
        CursorHistory history[255];
        SimplyNumbers *simplyGen;
        Hashtable *movesHash;
        TMove moves[fsize*fsize];

public:
        Cursor(SimplyNumbers *simplyGen, Hashtable *movesHash);
        CursorHistory *current();
        bool forward(TMove N);
        bool back();
        bool isAlllowed(TMove N);


        int count;

        unsigned char kl[fsize*fsize]; // 0 == empty, disabled
                                       // 1 == empty, enabled
// 0 == 'x',first

};


#endif
