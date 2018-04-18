//---------------------------------------------------------------------------
#ifndef hashtableH
#define hashtableH
//---------------------------------------------------------------------------
#include "hashtable.h"
#include "tnode.h"
#include "Logger.h"

//103x105x331 ->623 M

    #define hashTableSizeX 103
    #define hashTableSizeO 331
    #define hashTableSizeZ 103
    class Hashtable {
        private:
        TNode **table;
        Logger *logger;

        public:
        Hashtable(Logger *logger);
//        void put(TNode *node);
        TNode *get(THash hX, THash hO, int age);
        TNode *getOrCreate(THash hX, THash hO, int age, bool &created);
    };

//---------------------------------------------------------------------------
#endif
