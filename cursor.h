//---------------------------------------------------------------------------

#ifndef cursorH
#define cursorH
#include "tnode.h"
#include "SimplyNumbers.h"
#include "Hashtable.h"

#define fsize 15  //field size
#define TOTAL_CELLS 225 //fsize*fsize, cells total

#define MAX_ENABLERS 16
//---------------------------------------------------------------------------

class Cursor {

  struct CursorHistory {
    CursorHistory();
    TMove en[MAX_ENABLERS],//"Enabled" moves buffer
          move;
    int enCount;//count of actually "Enabled" moves
    int symmX, symmY, symmXY, symmW, symmXW, symmYW, symmXYW;
    TNode *node;
  };

private:
protected:
        CursorHistory history[TOTAL_CELLS];
        SimplyNumbers *simplyGen;
        Hashtable *movesHash;
        TMove moves[fsize*fsize];

public:
        Cursor(SimplyNumbers *simplyGen, Hashtable *movesHash);
        inline CursorHistory *current();
        bool forward(TMove N);
        bool back();
        bool isAlllowed(TMove N);


        int count;

        unsigned char kl[fsize*fsize]; // 0 == empty, disabled
                                       // 1 == empty, enabled
// 0 == 'x',first

};


#endif
