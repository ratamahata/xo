//---------------------------------------------------------------------------
#ifndef hashtableH
#define hashtableH
//---------------------------------------------------------------------------
#include "hashtable.h"
#include "tnode.h"
#include "Logger.h"

    // 10000*900 -> 0/298K collisions
    // 10002*902 -> 0/47960 collisions
    // 10004*904 -> 0/1M collisions
    // 30000*300 -> 59/23256 bad..
    // 10001*901 -> 0/10M
    #define hashTableSizeX 10000
    #define hashTableSizeO 902
    class Hashtable {
        private:
        TNode **table;
        Logger *logger;

        public:


        Hashtable(Logger *logger);
        void put(TNode *node);
        TNode *get(THash h1, THash h2, int age);
    };

//---------------------------------------------------------------------------
#endif
