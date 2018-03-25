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
    /*
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
    }*/
};



//=============================================================================
void Relator::updateParents(int addedChilds) {
    for(int i = 0; i < count-1; ++i) {
        updateParents(current()->node, i, 0, count, count-1, addedChilds);
    }
};

//=============================================================================
void Relator::updateParents(TNode *node, int depth, int removed, int maxEven, int maxOdd, int addedChilds) {
    bool odd = removed%2;
    int i = odd ? maxOdd : maxEven;
    for(; i > 1; i -= 2) {
        if (!history[i-1].removed) {
            TNode *n = getParent(node, history[i-1].move);
            if (n != NULL) {
                if (depth > removed) {
                    history[i-1].removed = true;
                    if (odd) {
                        maxOdd = i-2;
                    } else {
                        maxEven = i-2;
                    }
                    updateParents(n, depth, removed+1, maxEven, maxOdd, addedChilds);
                    history[i-1].removed = false;
                } else {
                    updateNode(n, node, addedChilds);
                }
            } else {
            //no parent found
            n=n;
            }
        }
    }
};

void Relator::updateNode(TNode *node, TNode *from, int addedChilds) {
//TODO can we update only from "from", so no iteration needed ?
/*
    short int max_rating = -32600;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==0) {
            continue;
        } else if (kl[i]>1) {
            bool occupied = false;
            for (int j = 0; j = count-1; ++j) {
                if (history[j].move == i && !history[j].removed) {//WTf ??
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
    }*/
    if (from->rating > -node->rating) {
        node->rating = - from->rating;
    }
    node->totalChilds += addedChilds;
}

//---------------------------------------------------------------------------

TNode* Relator::getParent(TNode *node, TMove move) {
    //bool lastMoveX = 0 == node->age % 2;
    //THash lastPos = lastMoveX ? node->hashCodeX : node->hashCodeO;
    unsigned long multiplier = simplyGen->getExistingHash(move);
    THash prevPos = node->hashCodeO / multiplier;
    THash remainder = node->hashCodeO % multiplier;
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
        prevPos = n*m2 + (n*r1 + prevPos) / multiplier;
        //THash remainder2 = (THASH_MAX % multiplier) + 1;
        //ToDo: check that (remainder + remainder2) == multiplier
    }
    THash hashCodeX = prevPos;
    THash hashCodeO = node->hashCodeX;
    TNode* n = movesHash->get(hashCodeX, hashCodeO, node->age - 1);
    return n;
};

//----------------------------------------------------------------------------

void Relator::findMovesToExpand() {//TODO use single iteration
    newChilds.count = 0;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==1 && isAlllowed(i)) {
                newChilds.move[newChilds.count++] = i;
        }
    }
};


