//---------------------------------------------------------------------------

#ifndef RelatorH
#define RelatorH
//---------------------------------------------------------------------------
#include "Evaluator.h"

class Relator : public Evaluator {

        public:
        void calculateChilds();
        RelativeBucket childs;

//-------------------------------------
        protected:

        class MovesBucket {
                public:
                int count;
                TMove move[MAX_RELATIVES];
        };

        Relator(SimplyNumbers*, Hashtable* );

        MovesBucket newChilds;
        TNode* getChild(TNode *parent, TMove childMove);
        void findMovesToExpand();
        Cursor::RelativeBucket getParents(TNode *node);
        void updateParents(int addedChilds);

//-------------------------------------
        private:

        //these are used fro parent calculation
//        void updateParents(int childsAdded);
        void updateParents(TNode *node, int depth, int removed, int maxEven, int maxOdd, int addedChilds);
        void updateNode(TNode *node, TNode *from, int addedChilds);
        TNode* getParent(TNode *node, TMove move);
        int minRemovedEven;
        int minRemovedOdd;

};

#endif
