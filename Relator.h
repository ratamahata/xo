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

        Cursor::RelativeBucket getParents(TNode *node);
        void calculateChilds();
        void findMovesToExpand();
        void updateParents();

//-------------------------------------
        private:
        inline bool unique(TMove move);
        inline bool allow(int move);

        //these are used fro parent calculation
        void updateParents(TNode *node, int depth, int removed);
        TNode* getParent(TNode *node, TMove move);
        int minRemovedEven;
        int minRemovedOdd;

};

#endif
