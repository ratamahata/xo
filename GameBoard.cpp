//---------------------------------------------------------------------------


#pragma hdrstop

#include "GameBoard.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


GameBoard::GameBoard(SimplyNumbers *simplyGen, Hashtable *movesHash,
                 bool *swapXp, bool *swapYp, bool *swapWp, int gameMode) : Builder(simplyGen, movesHash, gameMode) {
        swapX = swapXp;
        swapY = swapYp;
        swapW = swapWp;
};

bool GameBoard::put(TMove N) {
                int x = N%fsize - 7;
                int y = N/fsize - 7;

                if ((  history[count].symmX  == 0 || history[count].symmXYW  == 0
                   || history[count].symmXW  == 0 || history[count].symmXY  == 0) && x < 0) {
                        x = -x;
                        *swapX = true;
                }
                if ((  history[count].symmY  == 0 || history[count].symmXYW  == 0
                   || history[count].symmYW  == 0 || history[count].symmXY  == 0) && y < 0) {
                        y = -y;
                        *swapY = true;
                }
                if ((  history[count].symmW  == 0 || history[count].symmXYW  == 0
                   || history[count].symmXW  == 0 || history[count].symmYW  == 0) && x < y) {
                        int t = x;
                        x = y;
                        y = t;
                        *swapW = true;
                }
                return forward(y*15+x);
};


int GameBoard::move() {
//TODO
/*
 int f0 = -99999;
  TNode *choosen = 0;
  TNodeLink *choosenLink = 0;
  int totl = cursorLink->node->totalDirectChilds;
  if (totl == 0) {
        return -32600;
  }

  for (int i = 0; i < totl; ++i) {
    TNodeLink *link = &(cursorLink->node->childs[i]);
    TNode *node = link->node;
    int f = node->chooseFactor(node);
    if (f>f0 || f0 == -99999) {
      choosen = node;
      choosenLink = link;
      f0=f;
    }
  }
  if (choosen == NULL) {
  //should never happen??
        return -32600;
  }

  lastmove=choosen;
  put(choosenLink);
  return choosen->rating;
  */

  return 0;
};
