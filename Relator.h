//---------------------------------------------------------------------------

#ifndef RelatorH
#define RelatorH
//---------------------------------------------------------------------------
#include "Evaluator.h"

class Relator : public Evaluator {

#define MAX_RELATIVES 200

        class RelativeBucket {
                public:
                int count;
                TNode* node[MAX_RELATIVES];
                TMove move[MAX_RELATIVES];
        };

        protected:
        RelativeBucket childs, parents;

        void calculateChilds();
        void calculateParents();
};

#endif
