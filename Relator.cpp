//---------------------------------------------------------------------------


#pragma hdrstop

#include "Relator.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


void Relator::calculateChilds() {
    childs.count = 0;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==1) {

            THash hashCodeX = current()->node->hashCodeO;
            THash hashCodeO = current()->node->hashCodeX * simplyGen->getHash(i);

            TNode* n = movesHash->get(hashCodeX, hashCodeO, current()->node->age + 1);
            childs.move[childs.count] = i;
            childs.node[childs.count++] = n;
        }
    }
};

//---------------------------------------------------------------------------

void Relator::calculateParents(TNode *node) {

    RelativeBucket &parents = history[node->age].parents;
    parents.count = 0;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==1) {

            THash hashCodeO = node->hashCodeX;

            THash currO = node->hashCodeO;
            unsigned long multiplier = simplyGen->getHash(i);

            THash hashCodeX = currO / multiplier;
            THash remainder = currO % multiplier;
            if (remainder > 0) {
                unsigned long k = 0, n, t, r3;
                unsigned long m2 = THASH_MAX / multiplier;
                unsigned long r1 = (THASH_MAX % multiplier) + 1;
                unsigned long r2 = remainder % multiplier;

                if (multiplier % r1) {
                        for(;;) {
                                 ++k;
                                 t = multiplier*k - r2;
                                 n = t / r1;
                                 r3= t % r1;
                                 if (r3 == 0) {
                                        break;
                                 }
                        }
                }

                hashCodeX = n*m2 + (n*r1 + currO) / multiplier;

                //THash remainder2 = (THASH_MAX % multiplier) + 1;
                //ToDo: check that (remainder + remainder2) == multiplier
            }


            TNode* n = movesHash->get(hashCodeX, hashCodeO, node->age - 1);
            if (n != NULL) {
                parents.move[parents.count] = i;
                parents.node[parents.count++] = n;
            }
        }
    }
};

//----------------------------------------------------------------------------

void Relator::findMovesToExpand() {
    newChilds.count = 0;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==1) {
                newChilds.move[newChilds.count++] = i;
        }
    }
};

//------------------------------------------------------------------------------

inline bool Relator::unique(TMove move) {
//        if (history[count-1].symmX && history[count-1].symmY && history[count-1].symmXY && history[count-1].symmW
//                //&& history[count-1].symmXW && history[count-1].symmYW && history[count-1].symmXYW
//                ) {
//                return childCount;
//        }

                int x = move%fsize - 7;
                int y = move/fsize - 7;

                return (history[count-1].symmX  != 0 || x >= 0)
                              && (history[count-1].symmY  != 0 || y >= 0)
                              //&& (history[count-1].symmXY != 0 || x + y >= 0)
                              && (history[count-1].symmW != 0 || x >= y)
                              //&& (history[count-1].symmXW  != 0 || ((x>=0 || y<=0) && (x<=0 || y>=)0))
                              //&& (history[count-1].symmYW != 0 || ((x>=0 || y<=0) && (x<=0 || y>=0)))
                              && (history[count-1].symmXYW != 0 || x + y  >= 0)
                              && (allow(move));
}

//------------------------------------------------------------------------------

//initial implementation for supporting of prohibited moves for
//balanced games, currently supported 1st player
//to make 2nd move in central 5x5 square
inline bool Relator::allow(int move) {
//  if (gameMode == 1 && this->cursorLink->node->age == 1) {
//        int x = move % 15 - 7, y = move / 15 - 7;
//        return ! (x < 3 && x > -3 && y < 3 && y > -3);
//  }
  return true;
}



