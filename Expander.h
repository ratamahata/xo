//---------------------------------------------------------------------------

#ifndef ExpanderH
#define ExpanderH
//---------------------------------------------------------------------------

#include "Relator.h"

class Expander : public Relator {

public:
        int max_count;

protected:
        Expander(SimplyNumbers *simplyGen, Hashtable *movesHash);
        void expand();

};



#endif
