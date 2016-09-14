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

        void calculateChilds();
        void findMovesToExpand();
        void calculateParents(TNode *node);

//-------------------------------------
        private:
        inline bool unique(TMove move);
        inline bool allow(int move);


};

#endif
