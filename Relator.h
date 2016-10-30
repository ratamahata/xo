//---------------------------------------------------------------------------

#ifndef RelatorH
#define RelatorH
//---------------------------------------------------------------------------
#include "Evaluator.h"

class Relator : public Evaluator {

//-------------------------------------
        protected:

        class MovesBucket {
                public:
                int count;
                TMove move[MAX_RELATIVES];
        };

        Relator(SimplyNumbers*, Hashtable* );
        RelativeBucket childs;
        MovesBucket newChilds;

        TNode* getChild(TNode *parent, TMove childMove);
        void calculateChilds();
        void findMovesToExpand();
        Cursor::RelativeBucket getParents(TNode *node);
        void updateParents();

//-------------------------------------
        private:
        inline bool unique(TMove move);
        inline bool allow(int move);

        //these are used fro parent calculation
        void updateParents(int childsAdded);
        void updateParents(TNode *node, int depth, int removed, int maxEven, int maxOdd, int addedChilds);
        void updateNode(TNode *node, TNode *from, int addedChilds);
        TNode* getParent(TNode *node, TMove move);
        int minRemovedEven;
        int minRemovedOdd;

};

#endif
