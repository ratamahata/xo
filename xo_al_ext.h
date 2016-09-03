//---------------------------------------------------------------------------
#ifndef xo_al_extH
#define xo_al_extH
//---------------------------------------------------------------------------
#include "xo_al.h"
#include "tnode.h"
#include "hashtable.h"
#include "SimplyNumbers.h"

#define MAX_UPDATED_PARENTS 2000

#define FIRST_RATING 2350


class Al_ext : public Al{

  public:


  TNodeLink *first,//absolute begining of tree
    *cursorLink;

  TNode *lastmove;//last move in real game

  Hashtable *movesHash;

  int max_count;
  unsigned int recalc_deviation;
  unsigned int calc_deviation;
  unsigned int recalc_deviation_count;
  unsigned int calc_deviation_count;

  unsigned long rateTime;

  //bool balance;
  int gameMode;//0 = Go - Moku, 1 = 5-in-a-row, 2 = Renjue

  int updatedParentsCounter;
  TNode *updatedParents[MAX_UPDATED_PARENTS];

  SimplyNumbers *simplyGen;

  //  static Al_ext* instance;

  Al_ext(int gameMode);
  TNode* getFirstNode();
  TNode* createNode(THash hashCodeX, THash hashCodeO, int age, TRating rating);
  void rate(TNode *src, TNodeLink *dest); //fills {totalRating,x3,x4,o3,o4} of dest;
  short int put(TNodeLink *link);
  int put(int N, bool& swapX, bool& swapY, bool& swapW);  //player's move, 1 == wins
  int move();    //al's move
  void takeback();
  void restart();
  int getList(TNode *node, TByte *buf);
  int refineList(TByte *buf, int count);
  bool allow(int move);
//  TNode* findNode(TNodeLink *link, int distance, unsigned long hashCodeX, unsigned long hashCodeO);
  void expand();//create and fill childs
#ifdef DEBUG_VER
  bool verify(TNode *node, TByte move);
#endif
  void recalc_rating(int count);
  void updateParents(TNode *node, int childsAdded, short int oldRatingNeg);//updates rating and total childs
  void updateNode(TNode *node, int childsAdded);
  TNodeLink *chooseNodeToExpand();
  void buildTree();
  int cleanTree(TNode *node);

};


#endif
