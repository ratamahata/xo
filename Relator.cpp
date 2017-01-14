//---------------------------------------------------------------------------


#pragma hdrstop

#include "Relator.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Relator::Relator(SimplyNumbers* sn, Hashtable* ht) : Evaluator(sn, ht){};

TNode* Relator::getChild(TNode *parent, TMove childMove) {
   THash hashCodeX = parent->hashCodeO;
   THash hashCodeO = parent->hashCodeX * simplyGen->getHash(childMove);
   return movesHash->get(hashCodeX, hashCodeO, parent->age + 1);
}

void Relator::calculateChilds() {
    childs.count = 0;
    TNode *node = current()->node;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==1) {
            TNode* n = getChild(node, i);
            if (n != NULL) {
                childs.move[childs.count] = i;
                childs.node[childs.count++] = n;
            }
        }
    }
    if (childs.count == 0) {



    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==1) {
            TNode* n = getChild(node, i);
            if (n != NULL) {
                childs.move[childs.count] = i;
                childs.node[childs.count++] = n;
            }
        }
    }



      return;
    }
};



//=============================================================================
void Relator::updateParents(int addedChilds) {
    for(int i = 0; i < count-1; ++i) {
        updateParents(current()->node, i, 0, count-2, count-1, addedChilds);
    }
};

//=============================================================================
void Relator::updateParents(TNode *node, int depth, int removed, int maxEven, int maxOdd, int addedChilds) {
    bool odd = removed%2;
    int i = odd ? maxOdd : maxEven;
    for(; i > 0; i -= 2) {
        if (!history[i].removed) {
            TNode *n = getParent(node, history[i].move);
            if (n != NULL) {
                if (depth > removed) {
                    history[i].removed = true;
                    if (odd) {
                        maxOdd = i-2;
                    } else {
                        maxEven = i-2;
                    }
                    updateParents(n, depth, removed+1, maxEven, maxOdd, addedChilds);
                    history[i].removed = false;
                } else {
                    updateNode(n, node, addedChilds);
                }
            }
        }
    }
};

void Relator::updateNode(TNode *node, TNode *from, int addedChilds) {
    short int max_rating = -32600;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==0) {
            continue;
        } else if (kl[i]>1) {
            bool occupied = false;
            for (int j = 0; j = count-1; ++j) {
                if (history[j].move == i && !history[j].removed) {
                    occupied = true;
                    break;
                }
            }
            if (occupied) {
                continue;
            }
        }
        TNode *child = getChild(node, i);
        if (child != NULL && child->rating > max_rating) {
            max_rating = child->rating;
        }
    }
    node->rating = - max_rating;
    node->totalChilds += addedChilds;
}

//---------------------------------------------------------------------------

TNode* Relator::getParent(TNode *node, TMove move) {
    THash hashCodeO = node->hashCodeX;
    THash currO = node->hashCodeO;
    unsigned long multiplier = simplyGen->getHash(move);//TODO get only existing hash, so new simply number cab't be generated by this call
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
    return n;
};

//----------------------------------------------------------------------------

void Relator::findMovesToExpand() {//TODO use single iteration
    newChilds.count = 0;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==1 && unique(i)) {
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
  if (gameMode == 1 && this->lastMove()->age == 1) {
        int x = move % 15 - 7, y = move / 15 - 7;
        return ! (x < 3 && x > -3 && y < 3 && y > -3);
  }
  return true;
}



