//---------------------------------------------------------------------------

#ifndef EvaluatorH
#define EvaluatorH

#include "SimplyNumbers.h"
#include "Hashtable.h"
#include "cursor.h"
//---------------------------------------------------------------------------

class Evaluator : public Cursor {
        public:
        Evaluator(SimplyNumbers *simplyGen, Hashtable *movesHash);
        inline bool Evaluator::comp(int x, int y, unsigned char c);
        bool __fastcall Evaluator::scanlines(int BlNo, int &lines, int N);
        void Evaluator::rate(TNode *src, TNode *destNode, TMove move); //fills {totalRating,x3,x4,o3,o4} of dest;
};
#endif
