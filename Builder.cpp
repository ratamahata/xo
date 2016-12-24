//---------------------------------------------------------------------------


#pragma hdrstop

#include "Builder.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Builder::Builder(SimplyNumbers* s, Hashtable* h, int gameMode)
        : Expander(s, h){

    this->gameMode = gameMode;
    forward(112);
    expand();
};


//==================================================================

void Builder::buildTree() {
  //assert cursor==lastmove;

  int count0 = count;
  while (current()->node->totalDirectChilds > 0 && count < 224) {
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

int Builder::chooseNodeToExpand() {
  //TNode *cursor = current()->node;
  float f0 = -99999;
//  float f01 = -99999;

  int choosen = -1;
//  int choosen1 = -1;
  //int choosen1prev = -1;

  calculateChilds();
//  do {
        for (int i = 0; i < childs.count; ++i) {
            TNode *node = childs.node[i];
            int ttc = node->ratingToTotalChilds();
            int ret = (node->totalChilds > 60000) ? ttc*ttc : ttc;
            float f = ret/ (float)(1+node->totalChilds);
//            if (node->totalChilds > 500000) {
//                f = f*ttc;
//            }
            if (f > f0 || f0 == -99999) {
              choosen = i;
              f0 = f;
            }
//            if (ret > f01) {
//                choosen1prev = choosen1;
//                choosen1 = i;
//                f01 = ret;
//            }
        }
//          if (choosen1->node->rating < 10000 && choosen1->node->rating > -10000 && !(first->node->totalChilds % 5)) {
//                        if (choosen1prev != NULL && choosen1prev->node->rating < 10000 && choosen1prev->node->rating > -10000) {
//                                double f1prev = choosen1prev->node->ratingToTotalChilds(first->node) / (double)(1+choosen1prev->node->totalChilds);
//                                double f1 = choosen1->node->ratingToTotalChilds(first->node) / (double)(1+choosen1->node->totalChilds);
//                                choosen = f1 >= f1prev ? choosen1 : choosen1prev;
//                        } else {
//                                choosen = choosen1;
//                        }
//          }

//          if (cursorLink->node != lastmove) {
//                if (choosen->node->rating >= 32300 || choosen->node->rating <= -32300) {
//                    //bad situation
//                    //we add 10 more fake childs to such node, so it does not want to expand more..
//                    if (lastmove->rating < 32300 && lastmove->rating > -32300) {
//                            updatedParentsCounter = 0;
//                            updateNode(choosen->node, 10);
//                    }
//                    continue;
//                }
//        }
//  }
//  while(false);

  return choosen;
};

//==================================================================

