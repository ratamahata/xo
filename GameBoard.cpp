//---------------------------------------------------------------------------


#pragma hdrstop

#include "GameBoard.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


GameBoard::GameBoard(SimplyNumbers *simplyGen, Hashtable *movesHash,
                 bool *swapXp, bool *swapYp, bool *swapWp, bool *swapXYWp, int gameMode) : Builder(simplyGen, movesHash, gameMode) {
        swapX = swapXp;
        swapY = swapYp;
        swapW = swapWp;
        swapXYW = swapXYWp;

    TNode *node = createNode(1, 1, 0);
    forward(112, node);
    expand();
};

bool GameBoard::put(TMove N) {
                int x = N%fsize - 7;
                int y = N/fsize - 7;
                int absX = x < 0 ? -x : x;
                int absY = y < 0 ? -y : y;
                bool symmW = history[count-1].symmW;
                bool swapped = false;

                if (*swapW) {
                        int t = x;
                        x = y;
                        y = t;
                        swapped = true;
                }

                if (history[count-1].symmX  == 0) {
                    if (x < 0) {
                        x = -x;
                        *swapX = !*swapX;
                    }
                }
                if ((  history[count-1].symmY  == 0 || history[count].symmXY  == 0) && y < 0) {
                        y = -y;
                        *swapY = !*swapY;
                }

                if ( symmW  == 0 && absX < absY) {
                        int t = x;
                        x = y;
                        y = t;
                        *swapW = !*swapW;
                }

                if (swapped) {
                        int t = x;
                        x = y;
                        y = t;
                }

                //*swapX = *swapY = *swapXYW = *swapW = false;
                        /*
                if (x!=0 && x==-x0) {
                  *swapX = !*swapX;
                }
                if (y!=0 && y==-y0) {
                  *swapY = !*swapY;
                }
                if (x != y) {
                  if (x == y0 && y == x0) {
                    *swapW = !*swapW;
                  }
                  if (x == -y0 && y == -x0) {
                    *swapXYW = !*swapXYW;
                  }
                }         */

                return forward((y+7)*15+(x+7));
};


int GameBoard::move() {

  TNode *choosen = 0;

  int totl = current()->node->totalDirectChilds;
  if (totl == 0) {
        return -32600;
  }

    TNode *node = current()->node;
    int choosenMove;
    int rating;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==1) {
            TNode* n = getChild(node, i);
            if (n != NULL && (choosen == NULL || rating < n->chooseFactor(n))) {
                choosenMove = i;
                choosen = n;
                rating = choosen->chooseFactor(choosen);
            }
        }
    }

  if (choosen == NULL) {
  //should never happen??
        return -32600;
  }

  forward(choosenMove, choosen);
  return choosen->rating;
};
