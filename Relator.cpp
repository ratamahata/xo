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
    updateParents(current()->node, 0, 0, true, true, count-1, addedChilds);
};

//=============================================================================
//Private recursive method, used to update parent nodes of "node" by stages:
//"depth" parameter sets which parent level to update (0-parents, 1-grandparents and so on)
//"removed" parameter sets a number of moves that was virually removed when calculating parent
//"max" - maximum removable index
void Relator::updateParents(TNode *node, int removed, int removedFromEnd,
                bool onlyLastRemoved, bool updateRating, int max, int addedChilds) {
    for(int i = max; i > 0; i -= 2) {
        TNode *parent = getParent(node, history[i].move);
        if (parent != NULL) {
            updateRating = updateNode(parent, updateRating, addedChilds, removedFromEnd);
            updateParents(parent, removed+1,
                onlyLastRemoved&& i == max
                    ? removedFromEnd+1
                    : removedFromEnd,
                onlyLastRemoved&& i == max
                    ? true
                    : false,
                updateRating, i-1, addedChilds);
        } else {
                if (i == max && onlyLastRemoved) {
                        logger->error("no parent found");
                        getParent(node, history[i].move);
                }
        }
    }
};

bool Relator::updateNode(TNode *node, bool updateRating, int addedChilds, int removedFromEnd) {
    bool ratingUpdated = false;
    if (updateRating || addedChilds == 0) {
        short int max_rating = -32600;
        for(int i = 0; i<count-removedFromEnd; ++i)
            for(int j = 0; j<history[i].enCount; ++j) {
            TNode *child = getChild(node, history[i].en[j]);
            if (child != NULL && child->rating > max_rating) {
                max_rating = child->rating;
            }
        }
        if (node->rating != -max_rating) {
            TRating absRatingOld = node->rating;
            TRating absRating = node->rating = -max_rating;

            if (node->totalChilds >= BIG_PARENT) {
                if (absRatingOld < 0) absRatingOld = -absRatingOld;
                if (absRating < 0) absRating = -absRating;

                if (absRatingOld < CULL_RATING1 && absRating >= CULL_RATING1) {
                        ++logger->bigParentsCulled1;
                        if (node->totalChilds >= BIG_GRAND_PARENT) {
                                ++logger->bigGrandParentsCulled1;
                        }
                } else if (absRatingOld < CULL_RATING2 && absRating >= CULL_RATING2) {
                        ++logger->bigParentsCulled2;
                        if (node->totalChilds >= BIG_GRAND_PARENT) {
                                ++logger->bigGrandParentsCulled2;
                        }
                }
            }
            ratingUpdated = true;
        }
    }
    node->totalChilds += addedChilds;
    return ratingUpdated;
}

//---------------------------------------------------------------------------

TNode* Relator::getParent(TNode *node, TMove move) {
    //bool lastMoveX = 0 == node->age % 2;
    //THash lastPos = lastMoveX ? node->hashCodeX : node->hashCodeO;
    unsigned long multiplier = simplyGen->getExistingHash(move);
    THash prevPos = node->hashCodeO / multiplier;
    THash remainder = node->hashCodeO % multiplier;

    bool overflow = false;

    if (remainder ==0 && multiplier==2) {
        TNode* n = movesHash->get(prevPos, node->hashCodeX, node->age - 1);
        if (n != NULL) {
                return n;
        }
        overflow = true;
    }

    if (remainder > 0 || overflow) {
        unsigned long k = 0, n, t, r3;
        unsigned long m2 = THASH_MAX / multiplier;
        unsigned long r1 = (THASH_MAX % multiplier) + 1;

        for(;;) {
            ++k;
            t = r1*k + remainder;
            n = t/multiplier;
            r3 = t%multiplier;
            if (0 == r3) {
                break;
            }
        }
        prevPos = k*m2 + prevPos + n;
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


