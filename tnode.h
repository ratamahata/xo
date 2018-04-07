//---------------------------------------------------------------------------
#ifndef tnodeH
#define tnodeH
//---------------------------------------------------------------------------
#include "tnode.h"

#define NULL 0

typedef unsigned char TByte;
typedef TByte TMove;
typedef unsigned int THash;
#define THASH_MAX 4294967295U //ULLONG_MAX from <limits.h>
typedef signed short int TRating;

    class TNode {
      public:
      static double avgDiff, avgSquareDiff;
      static long long updatesCount;
      static long long skippedCount;

      TNode();
      void update(short int newRating, unsigned int addedChilds);
      int chooseFactor(TNode *firstNode);
      int ratingToTotalChilds();
//      int removeChild(TNode *child);
//      void remove();
//      int checkChilds(int recursion);
//      int cleanChilds(int critRating, int recursion);


      unsigned int totalChilds;
      TRating rating;
//      TNode *firstParent;//deprecated
//      TNodeLink2 *nextParent;//deprecated
//      TNodeLink *childs;//deprecated
//      void *reserved;
      bool fixedRating;
      TByte //move,
        totalDirectChilds,
        x2,x3,x4,//checks after this move of opponent
        o2,o3,o4,//checks after this move of move-maker
        age;

      THash hashCodeX, hashCodeO;
      TNode *next;
    };


//---------------------------------------------------------------------------
#endif