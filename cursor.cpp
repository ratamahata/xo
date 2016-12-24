//---------------------------------------------------------------------------


//#pragma hdrstop

#include <mem.h>
#include "cursor.h"


//---------------------------------------------------------------------------

//#pragma package(smart_init)

Cursor::CursorHistory::CursorHistory() {
        setmem((void*)en, 0, sizeof(CursorHistory));
};

//-------------------------------  CURSOR -----------------------------------

Cursor::Cursor(SimplyNumbers *simplyGen, Hashtable *movesHash) {
        this->simplyGen = simplyGen;
        this->movesHash = movesHash;
        count = 0;
};

//=============================================================================

inline Cursor::CursorHistory *Cursor::current() {
        return &(history[count]);
}

TNode *Cursor::getFirstNode() {
        return history[0].node;
}

TNode *Cursor::lastMove() {
        return current()->node;
}

void Cursor::restart() {
  while(count > 1) {
     back();
  }
  //lastmove = first->node;
}



//=============================================================================
//Increment the Cursor to next move N. Returns true on success, otherwise - false.
bool Cursor::forward(TMove N) {

  TNode *node = current()->node;

  THash hashCodeX = node->hashCodeO;
  THash hashCodeO = node->hashCodeX * simplyGen->getHash(N);

  TNode *nextNode = movesHash->get(hashCodeX, hashCodeO, node->age + 1);
  if (nextNode == NULL) {
        return false;//no entry, return error
  }

  return forward(N, nextNode);
}

//=============================================================================
bool Cursor::forward(TMove N, TNode* node) {

//  if (kl[N] > 3) {//TODO
//    return (bool)N / 0;
//  }

  int x = N % 15, y = N / 15;


  //begin: forward cursor
  kl[N] = ((count&1)+1)<<2;
  moves [count] = N;
  ++count;

  //begin: update simmetries history
        int d = 0,i;
        d = x==7 ? 0 : (kl[y*15+(14-x)] == kl[N]) ? -1 : 1;
        current()->symmX = history[count-1].symmX + d;

        d = y==7 ? 0 : (kl[(14-y)*15+x] == kl[N]) ? -1 : 1;
        current()->symmY = history[count-1].symmY + d;

        i = (14-y)*15+(14-x);
        d = N==i? 0 : (kl[i] == kl[N]) ? -1 : 1;
        current()->symmXY = history[count-1].symmXY + d;

        d = x==y ? 0 : (kl[x*15+y] == kl[N]) ? -1 : 1;
        current()->symmW = history[count-1].symmW + d;

        i=(14-x)*15+y;
        d = N==i? 0 : (kl[i] == kl[N]) ? -1 : 1;
        current()->symmXW = history[count-1].symmXW + d;

        i=x*15+14-y;
        d = N==i? 0 : (kl[i] == kl[N]) ? -1 : 1;
        current()->symmYW = history[count-1].symmYW + d;

        i=(14-x)*15+14-y;
        d = N==i? 0 : (kl[i] == kl[N]) ? -1 : 1;
        current()->symmXYW = history[count-1].symmXYW + d;
  //end: update symmetries history

  //begin: update "enablers" history
    current()->enCount = 0;

    if (gameMode == 1 &&  lastMove()->age == 1) {
        for (int i=-3; i<=3; i++)
            for (int j=-3; j<=3; j++)
                if (i==-3||i==3||j==-3||j==3) {
                        int x1 = 7+i, y1 = 7+j;
                        if (x1>=0 && y1>=0 && x1<fsize && y1<fsize && kl[y1*fsize+x1] == 0) {
//                              history[count].en[t++] = (7+j)*fsize+7+i;
                                kl[y1*fsize+x1] = 1;
                                current()->en[current()->enCount++] = y1*fsize+x1;
                        }
                }
    } else {
        int max = count > 1 ? 3 : 2;
        for (int d=1; d<max; d++)
            for (int i=-1; i<2; i++)
              for (int j=-1; j<2; j++)
                if ((!i)&&(!j)) continue;
                else {
                    int x1 = x+i*d, y1 = y+j*d;
                    if (x1>=0 && y1>=0 && x1<fsize && y1<fsize && kl[y1*fsize+x1] == 0) {
                        kl[y1*fsize+x1] = 1;
                        current()->en[current()->enCount++] = y1*fsize+x1;
                    }
                }
    }
  //end: update "enablers" history

  return true;
};

//=============================================================================

//set cursor to one move back
bool Cursor::back() {
  if (count == 0) {
        return false;
  }
  kl[current()->move] = 1;
  for(int t=0; t<current()->enCount; t++)
        kl[current()->en[t]] = 0;
  --count;
  return true;
}

//=============================================================================

bool Cursor::isAlllowed(TMove N) {
                int x = N%fsize - 7;
                int y = N/fsize - 7;

                if ((  history[count].symmX  == 0 || history[count].symmXYW  == 0
                   || history[count].symmXW  == 0 || history[count].symmXY  == 0) && x < 0) {
                   return false;
                }
                if ((  history[count].symmY  == 0 || history[count].symmXYW  == 0
                   || history[count].symmYW  == 0 || history[count].symmXY  == 0) && y < 0) {
                   return false;                }
                if ((  history[count].symmW  == 0 || history[count].symmXYW  == 0
                   || history[count].symmXW  == 0 || history[count].symmYW  == 0) && x < y) {
                   return false;
                }
                return true;
};




