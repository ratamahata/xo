//---------------------------------------------------------------------------


#pragma hdrstop

#include "Expander.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Expander::Expander(SimplyNumbers *simplyGen, Hashtable *movesHash)
        : Relator (simplyGen, movesHash) {
  max_count = 0;
}

//==================================================================
//adds childs to cursor node
void Expander ::expand() {
  TNode* cursor = current()->node;
  if (cursor->rating >= 32300 || cursor->rating <= -32300) {
      ++cursor->totalDirectChilds;
      logger->error("can't expand");
      return;
  }
//  if (logger != NULL) {
//        logger->expand(count);
//  }
  findMovesToExpand();

  short int max_rating = -32600;

  for(int i=0; i<newChilds.count; ++i) {

    TMove move = newChilds.move[i];

    THash newHashX = cursor->hashCodeO;
    THash newHashO = cursor->hashCodeX * simplyGen->getHash(move);

    TNode *node = movesHash->get(newHashX, newHashO, cursor->age + 1);

    if (node == NULL) {
            node = createNode(newHashX, newHashO, cursor->age + 1);
            rate(cursor, node, move);
    } else {
        node = node;
    }

    if (node->rating > max_rating) max_rating = node->rating;
  }

  short int oldRating = cursor->rating;
  if (!cursor->fixedRating) {
        cursor->rating = (TRating)(0.4*(double)oldRating-0.6*(double)max_rating);
  }

    cursor->totalDirectChilds = cursor->totalChilds = newChilds.count;


  updateParents(newChilds.count);
// TODO:
//  updatedParentsCounter = 0;
//  updateParents(cursor, cursor->totalChilds, oldRating);
//  current()->

  if (count > max_count) max_count = count;
};

//==================================================================
TNode* Expander ::createNode(THash hX, THash hO, TByte age) {
    TNode *node = new TNode();
    node->hashCodeX = hX;
    node->hashCodeO = hO;
    node->age = age;
    movesHash->put(node);
    return node;
}
