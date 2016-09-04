//---------------------------------------------------------------------------


#pragma hdrstop

#include "Builder.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//Builder::Builder(SimplyNumbers* s, Hashtable* h);
//        : Relator(s, h){};

//==================================================================

void Builder::buildTree() {
  //assert cursor==lastmove;

  int count0 = count;
  while (current()->node->totalDirectChilds > 0 && count < 224) {
    calculateChilds();
    int i = chooseNodeToExpand();
    forward(childs.move[i], childs.node[i]);
  }

  expand();

  while(count>count0) back();

  /*
  //Auto cleaning (not works)
  int crit = first->node->rating*2;
  if (crit < 0) {
        crit = -crit;
  }
  if (crit > 3300 && (expanded->rating < -crit || expanded->rating > crit)) {
        while (expanded->firstParent->rating < -crit || expanded->firstParent->rating > crit) {
                expanded = expanded->firstParent;
        }
        expanded->cleanChilds(crit, 0);
  }
  */
  //assert cursor==lastmove;
}

//==================================================================
