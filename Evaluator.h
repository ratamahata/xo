//---------------------------------------------------------------------------

#ifndef EvaluatorH
#define EvaluatorH

#include "SimplyNumbers.h"
#include "Hashtable.h"
#include "Cursor.h"
//---------------------------------------------------------------------------

class Evaluator : public Cursor {

        public:
        Evaluator(SimplyNumbers* sn, Hashtable* ht);
        void rate(TNode *src, TNode *destNode, TMove move); //fills {totalRating,x3,x4,o3,o4} of dest;

        protected:
        bool  scanlines(int BlNo, int &lines, int N);
        int scanlines(int BlNo, int &lines, int N, TNode *destNode, int &nAttacks);

        private:
        int cnt;
        inline bool comp(int x, int y, unsigned char c);
        void addAttackPair(TNode* destNode, int x1, int y1, int x2, int y2, int &totalAttacks);
	bool isMoveBlockingAttack(const TAttack& atk, TMove m);
	TMove findNextFreeInward(TMove from, TMove target);
	void updateRange(int cIndex, int sdv, int x, int y, int vx, int vy, 
                  int &firstC, int &lastC);	
};
#endif
