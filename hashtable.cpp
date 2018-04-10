#include <mem.h>
#include "hashtable.h"

//------------------------------------------------------------------
Hashtable::Hashtable(Logger *logger) {
    table = new TNode*[hashTableSizeX*hashTableSizeO];
    memset(table, 0, sizeof(TNode*)*hashTableSizeX*hashTableSizeO);
    this->logger = logger;
}

//------------------------------------------------------------------
void Hashtable::put(TNode *node) {
    unsigned long h1 = node->hashCodeX % hashTableSizeX;
    unsigned long h2 = node->hashCodeO % hashTableSizeO;
    unsigned long index = h1*hashTableSizeO + h2;
    TNode **array = &(table[index]);

    while (array[0] != NULL) {
//        if (array[0]->hashCodeX == node->hashCodeX && array[0]->hashCodeO == node->hashCodeO) {
//          return;
//        }
        array = &((*array)->next);
    }
    array[0] = node;
}

//------------------------------------------------------------------
TNode *Hashtable::get(THash h1, THash h2, int age) {
    unsigned long index = (h1%hashTableSizeX)*hashTableSizeO + (h2%hashTableSizeO);
    TNode **array = &(table[index]);
    bool m2 = false;
    bool m3 = false;

    if (h1 ==0 || h2 ==0) {
        //never happens
        return NULL;
    }

    while (array[0] != NULL) {
        if (array[0]->hashCodeX == h1 && array[0]->hashCodeO == h2) {

          if (age == array[0]->age) {
                        logger->hit();
                        return array[0];
          } else {
                m3 = true;
          }
        } else {
                m2 = true;
        }
        array = &(array[0]->next);
    }

    if (m3) {
        logger->missAge();
    } else if (m2) {
        logger->missHash();
    } else {
        logger->missIndex();
    }
    return NULL;
};
