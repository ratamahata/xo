#include <mem.h>
#include "hashtable.h"

//------------------------------------------------------------------
Hashtable::Hashtable(Logger *logger) {
    table = new TNode**[hashTableSizeX*hashTableSizeO];
    memset(table, 0, sizeof(TNode**)*hashTableSizeX*hashTableSizeO);
    this->logger = logger;
}

//------------------------------------------------------------------
void Hashtable::put(TNode *node) {

    THash hashX = node->hashCodeX;
    unsigned long h1 = hashX / hashTableSizeZ;
    unsigned long zIndex = hashX % hashTableSizeZ;
    h1 = h1 % hashTableSizeO;
    unsigned long h2 = node->hashCodeO % hashTableSizeO;
    unsigned long index = h1*hashTableSizeO + h2;

    TNode **array = table[index];
    if (array == NULL) {
        table[index] = array = new TNode*[hashTableSizeZ];
        memset(array, 0, sizeof(TNode*)*hashTableSizeZ);
    }

    TNode **curr;
    curr = &(array[zIndex]);

    while (curr[0] != NULL) {
//        if (array[0]->hashCodeX == node->hashCodeX && array[0]->hashCodeO == node->hashCodeO) {
//          return;
//        }
        curr = &(curr[0]->next);
    }
    curr[0] = node;
}

//------------------------------------------------------------------
TNode *Hashtable::get(THash hX, THash hO, int age) {
//    unsigned long index = (h1%hashTableSizeX)*hashTableSizeO + (h2%hashTableSizeO);

    unsigned long h1 = hX / hashTableSizeZ;
    unsigned long zIndex = hX % hashTableSizeZ;
    h1 = h1 % hashTableSizeO;
    unsigned long h2 = hO % hashTableSizeO;
    unsigned long index = h1*hashTableSizeO + h2;

    TNode **array = table[index];
    bool m2 = false;
    bool m3 = false;

    if (hX ==0 || hO ==0) {
        logger->error("zero hashcode");
    }

    if (array != NULL) {

      TNode *curr = array[zIndex];

      while (curr != NULL) {

        if (curr->hashCodeX == hX && curr->hashCodeO == hO) {
          if (age == curr->age) {
                        logger->hit();
                        return curr;
          } else {
                m3 = true;
          }
        } else {
                m2 = true;
        }
        curr = curr->next;
      }
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
