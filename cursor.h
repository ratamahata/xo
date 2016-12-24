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

#define MAX_RELATIVES 224 // == (fsize*fsize - 1)

public:
        class RelativeBucket {
                public:
                int count;
                TNode* node[MAX_RELATIVES];
                TMove move[MAX_RELATIVES];
        };

  struct CursorHistory {
    CursorHistory();
    TMove en[MAX_ENABLERS],//"Enabled" moves buffer
          move;
    int enCount;//count of actually "Enabled" moves
    int symmX, symmY, symmXY, symmW, symmXW, symmYW, symmXYW;
    TNode *node;
    RelativeBucket parents;
    bool removed;
  };

  int gameMode;//0 = Go - Moku, 1 = 5-in-a-row, 2 = Renjue
  int count;//count of moves made

  TNode *getFirstNode();
  TNode *lastMove();
  void restart();
  bool back();

  Hashtable *movesHash;
  CursorHistory history[TOTAL_CELLS];


  SimplyNumbers *simplyGen;
  TMove moves[fsize*fsize]; //ordered history of moves

  TMove kl[fsize*fsize]; // moves on field


private:
protected:




        Cursor(SimplyNumbers *simplyGen, Hashtable *movesHash);
        inline CursorHistory *current();
        bool forward(TMove N);
        bool forward(TMove N, TNode* node);
        bool isAlllowed(TMove N);

};


#endif
