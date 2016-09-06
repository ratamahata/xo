//---------------------------------------------------------------------------

#ifndef BuilderH
#define BuilderH
//---------------------------------------------------------------------------
#include "Expander.h"




class Builder : public Expander {

        int chooseNodeToExpand();

        protected:
        Builder(SimplyNumbers*, Hashtable* );


        public:
        void buildTree();

};

#endif
