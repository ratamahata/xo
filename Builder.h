//---------------------------------------------------------------------------

#ifndef BuilderH
#define BuilderH
//---------------------------------------------------------------------------
#include "Relator.h"



class Builder : public Relator {

        int chooseNodeToExpand();
        void expand();

        protected:
        Builder(SimplyNumbers*, Hashtable* );


        public:
        void buildTree();

};

#endif
