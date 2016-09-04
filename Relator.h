//---------------------------------------------------------------------------

#ifndef RelatorH
#define RelatorH
//---------------------------------------------------------------------------
#include "Evaluator.h"

class Relator : public Evaluator {

#define MAX_RELATIVES 200

        class RelativeBucket {
                public:
                TNode* relative[MAX_RELATIVES];
                int count;
        };

        RelativeBucket childs, parents;

        public:
        void calculateChilds();
        void calculateParents();
};

#endif
