//---------------------------------------------------------------------------

#ifndef ExpanderH
#define ExpanderH
//---------------------------------------------------------------------------

#include "Relator.h"

class Expander : public Relator {


        protected:
        Expander(SimplyNumbers *simplyGen, Hashtable *movesHash);
        int max_count;
        void expand();

};



#endif
