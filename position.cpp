 #include "position.h"

Position::Position(SimplyNumbers *simplyGen, Hashtable *movesHash) {
        this->simplyGen = simplyGen;
        this->movesHash = movesHash;
        count = 1;
};

//handler of user's move
bool Position::forward(int N0, bool& swapX, bool& swapY, bool& swapW) {

  int N = N0;
  int x = N % 15, y = N / 15, attempt = 0;
  TNodeLink *link;
  bool swX = false, swY = false, swW = false;

  THash hashCodeX = node->hashCodeO;
  do {

    THash hashCodeO = node->hashCodeX * simplyGen->getHash(N);

    TNode *nextNode = movesHash->get(hashCodeX, hashCodeO, node->age + 1);
    if (nextNode != NULL) {
        node = nextNode;
        ++count;
        break;
    }

                if (attempt < 1 && history[count-1].symmX == 0 && x!= 7) {
                    N = y*15+(14-x);
                    attempt = 1;
                    swX = true; swY = false; swW = false;
                } else if (attempt < 2 && history[count-1].symmY == 0 && y != 7) {
                    N = (14-y)*15+x;
                    attempt = 2;
                    swX = false; swY = true; swW = false;
                } else if (attempt < 3 && history[count-1].symmW == 0 && x != y) {
                    N = x*15+y;
                    attempt = 3;
                    swX = false; swY = false; swW = true;
                } else if (attempt < 4 && history[count-1].symmXYW == 0 && x != -y) {
                    N = (14-x)*15+(14-y);
                    attempt = 4;
                    swX = true; swY = true; swW = true;
                } else {
                        return false;
                }
  } while (true);
  swapX = swapX ^= swW ? swY : swX;
  swapY = swapY ^= swW ? swX : swY;
  swapW = swapW ^= swW;


  if (count == 0) {
          history[count].symmX = 0;
          history[count].symmY = 0;
          history[count].symmW = 0;
          history[count].symmXYW = 0;
  } else {
        int d = 0,i;
        d = x==7 ? 0 : (kl[y*15+(14-x)] == kl[link->move]) ? -1 : 1;
        history[count].symmX = history[count-1].symmX + d;

        d = y==7 ? 0 : (kl[(14-y)*15+x] == kl[link->move]) ? -1 : 1;
        history[count].symmY = history[count-1].symmY + d;

//        i = (14-y)*15+(14-x);
//        d = link->move==i? 0 : (kl[i] == kl[link->move]) ? -1 : 1;
//        history[count].symmXY = history[count-1].symmXY + d;

        d = x==y ? 0 : (kl[x*15+y] == kl[link->move]) ? -1 : 1;
        history[count].symmW = history[count-1].symmW + d;

//        i=(14-x)*15+y;
//        d = link->move==i? 0 : (kl[i] == kl[link->move]) ? -1 : 1;
//        history[count].symmXW = history[count-1].symmXW + d;

//        i=x*15+14-y;
//        d = link->move==i? 0 : (kl[i] == kl[link->move]) ? -1 : 1;
//        history[count].symmYW = history[count-1].symmYW + d;

        i=(14-x)*15+14-y;
        d = link->move==i? 0 : (kl[i] == kl[link->move]) ? -1 : 1;
        history[count].symmXYW = history[count-1].symmXYW + d;
  }


  //assert cursor==lastmove;
  ++count;
  return true;
};


