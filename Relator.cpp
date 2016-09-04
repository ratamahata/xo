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
            if (n != NULL) {
                childs.relative[childs.count++] = n;
            }
        }
    }
};



//---------------------------------------------------------------------------

void Relator::calculateParents() {
    parents.count = 0;
    for (int i = 0; i < TOTAL_CELLS; ++i) {
        if (kl[i]==1) {

            THash hashCodeO = current()->node->hashCodeX;

            THash currO = current()->node->hashCodeO;
            unsigned long multiplier = simplyGen->getHash(i);

            THash hashCodeX = currO / multiplier;
            THash remainder = currO % multiplier;
            if (remainder > 0) {
                THash addendum = THASH_MAX / multiplier;
                hashCodeX += addendum;

                //THash remainder2 = (THASH_MAX % multiplier) + 1;
                //ToDo: check that (remainder + remainder2) == multiplier
            }


            TNode* n = movesHash->get(hashCodeX, hashCodeO, current()->node->age + 1);
            if (n != NULL) {
                parents.relative[childs.count++] = n;
            }
        }
    }
};

