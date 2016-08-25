#include <mem.h>
#include "hashtable.h"

//------------------------------------------------------------------
Hashtable::Hashtable() {
    table = new TNodeLink2*[hashTableSizeX*hashTableSizeO];
    memset(table, 0, sizeof(TNodeLink2*)*hashTableSizeX*hashTableSizeO);
    hit = 0;
    miss1 = 0;
    miss2 = 0;
    miss3 = 0;
    miss4 = 0;
}

//------------------------------------------------------------------
void Hashtable::put(TNode *node) {
    unsigned long h1 = node->hashCodeX % hashTableSizeX;
    unsigned long h2 = node->hashCodeO % hashTableSizeO;
    unsigned long index = h1*hashTableSizeO + h2;
    TNodeLink2 **array = &(table[index]);

    while (array[0] != NULL) {
        if (array[0]->node->hashCodeX == h1 && array[0]->node->hashCodeO == h2) {
          return;
        }
        array = &((*array)->next);
    }
    array[0] = new TNodeLink2();
    array[0]->node = node;
}

//------------------------------------------------------------------
TNode *Hashtable::get(THash h1, THash h2, int age) {
    unsigned long index = (h1%hashTableSizeX)*hashTableSizeO + (h2%hashTableSizeO);
    TNodeLink2 **array = &(table[index]);
    bool m2 = false;
    bool m3 = false;

    if (h1 ==0 || h2 ==0) {
        //never happens
        return NULL;
    }

    while (array[0] != NULL) {
        if (array[0]->node->hashCodeX == h1 && array[0]->node->hashCodeO == h2) {
          int age1 = array[0]->node->getAge();
          if (age1 == age) {
                        ++hit;
                        return array[0]->node;
          } else {
                m3 = true;
          }
        } else {
                m2 = true;
        }
        array = &(array[0]->next);
    }

    if (m3) {
        ++miss3;
    } else if (m2) {
        ++miss2;
    } else {
        ++miss1;
    }
    return NULL;
};
