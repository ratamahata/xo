//---------------------------------------------------------------------------
//#include <vcl.h>
#include <math.h>
#include <vcl.h>
#include "hashtable.h"
#pragma hdrstop

#include "xo_al_ext.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

///------------------------------------------------------------
///                     the Model (data)
///------------------------------------------------------------


double TNode::avgDiff = 0;
double TNode::avgSquareDiff = 0;
long long TNode::updatesCount = 0;
long long TNode::skippedCount = 0;

Al_ext::Al_ext(int gameMode) {
    rateTime = 0;
    simplyGen = new SimplyNumbers();
    this->gameMode = gameMode;
    movesHash = new Hashtable();
    max_count = 0;
    cursorLink = first = new TNodeLink();
    first->node = lastmove = new TNode();
    first->node->age = count;
    first->node->rating = gameMode>0 ? 0 : FIRST_RATING;
    first->move = 112;
    put(first);
    expand();
};

TNode *Al_ext::getFirstNode() {
        return first->node;
}

TNode *Al_ext::createNode(THash hashCodeX, THash hashCodeO, int age, TRating rating) {
    TNode *node = new TNode();
    node->hashCodeX = hashCodeX;
    node->hashCodeO = hashCodeO;
    node->rating = rating;
    node->age = age;
    node->fixedRating = true;
    movesHash->put(node);
    return node;
}


//==================================================================

void Al_ext::rate(TNode *src, TNodeLink *dest) { //fills {totalRating,x3,x4,o3,o4} of dest;
  static const int vec[4][2] = {{1,1},{1,-1},{1,0},{0,1}};

  unsigned long beginTime = GetTickCount();

  TNode *destNode = dest->node;

  destNode->hashCodeX = src->hashCodeO;
  destNode->hashCodeO = src->hashCodeX * simplyGen->getHash(dest->move);

  destNode->o2 = src->x2;
  destNode->o3 = src->x3;
  destNode->o4 = src->x4;
  destNode->x2 = src->o2 > 1 ? src->o2 - 1 : src->o2;
  destNode->x3 = src->o3;
  destNode->x4 = src->o4;

  int t0, t = 15; // 'lines'

  if (destNode->o4 > 0)// построить 5
    if (scanlines(0, t, dest->move)) {
      destNode->rating = 32600;
      return;
    } else {
        //should never happen (?)
        t=t;
    }
  int ret = 0;
  bool c3my = false;
  if (destNode->x4 > 0)//закрыть 4
    if (scanlines(1, t, dest->move)) {
        --destNode->x4;
        c3my = 1;
      }

  if (destNode->o3 > 0) {//постр открытую 4
    t0 = t;
    if (scanlines(2, t, dest->move)) {
      destNode->o4 += 2;
      --destNode->o3;
      last4L = t0 - t;
    }
  }
  bool c3 = true;
  while (destNode->x3&&c3) {//закрыть 3
    int t0 = t;
    if (c3 = scanlines(3, t, dest->move)) {
        --destNode->x3;
        int tdiff = t0-t;
        int nvec = tdiff == 1 ? 0
                : tdiff == 2 ? 1
                : tdiff == 4 ? 2
                : tdiff == 8 ? 3
                : -1;//should not happen
        if (nvec < 0) {//Die!
                nvec = nvec / 0;
        }
        int x = dest->move % 15, y = dest->move / 15;
        bool inside = kl[x + vec[nvec][0] + 15*(y + vec[nvec][1])]
                == kl[x - vec[nvec][0] + 15*(y - vec[nvec][1])];
        if (!inside) {
                //closed 3 is built
                destNode->x2 += 12;
        }
        ret = -600;//this is helpful for some reason
      }
  }


/*
  if (count == 1) {
    if (scanlines(11, t, dest->move)) {//второй ход
        destNode->rating = 200;
    } else {
        destNode->rating = 0;
    }
    return;
  }*/

  //t = 15;
  t0 = t;
  while (scanlines(4, t, dest->move)) {//построить закрытую 4
    ++destNode->o4;
    destNode->o2 -= destNode->o2 >= 12 ? 12 : destNode->o2;
    if (c3my) c3my = 2;
    last4L = t0 - t;
  }

  while (scanlines(5, t, dest->move)) {//постр 3
    destNode->o3 += 1;
    destNode->o2 -= destNode->o2 >= 10 ? 10 : destNode->o2;
  }

  while (destNode->x2 > 0 && scanlines(9, t, dest->move)) {//закрыл 2
    destNode->x2 -= destNode->x2 >= 9 ? 8 : (destNode->x2+1)/2; //>= 2 ? 2 : destNode->x2 >= 1 ? 1 : 0;
  }

  while (scanlines(10, t, dest->move)) {//постр 2
    destNode->o2 += 10;
  }

        double k2 = destNode->age > 12 ? 0.1
                : destNode->age > 10 ? 0.2
                : destNode->age > 7 ? 0.4
                : destNode->age > 4 ? 0.7
                : destNode->age > 2 ? 0.9
                : 1;

        if (t > 0) {
            double k1 = destNode->age > 14 ? 0.1
                : destNode->age > 12 ? 0.3
                : destNode->age > 10 ? 0.5
                : destNode->age > 8 ? 0.7
                : destNode->age > 6 ? 0.8
                : destNode->age > 4 ? 0.9
                : 1;

            while (scanlines(11, t, dest->move)) {//ToT
                if (id) {
                        ret += (int)(220*k1);
                }
                else {ret -= (int)(110*k1);}
            }
        }
        ret -= src->rating*k2;


  /* // hz hz
  if ((c3my==2)&&(dest->o3)) {
    int i=0, n15 = 15;
    for (int N = 0; N<fsize*fsize; ++N)
      if (kl[N]&3) {
         while (scanlines(2, n15, N)) ++i;
         n15 = 15;
      }
    dest->o3 = i;
  }*/

exit:

//begin check special cases
//  if (destNode->age==4 && destNode->o2 >= 25 && destNode->x2 <= 5) {
//        ret = 6026;
        //destNode->fixedRating = true;
  //} else
  if (destNode->age==5) {
        int m1 = ((TNodeLink*)(this->history[1].link))->move;
        int m2 = ((TNodeLink*)(this->history[3].link))->move;
        int m3 = dest->move;
        if (m1/15 == m2/15 && m1/15 == m3/15 || m1%15 == m2%15 && m1%15 == m3%15) {
          ret = -6026;
          destNode->fixedRating = true;
        }
  }
//end check special cases

  if (destNode->fixedRating) {}
  else if (destNode->x4) {
    ret = -32200;
  } else if (destNode->x3 > 0 && destNode->o4 == 0) {
    ret = -30000;
  } else {
    ret +=destNode->o4*4200
        + destNode->o3*3840
        + destNode->o2*84
        - destNode->x3*6000
        - destNode->x2*150
      /*- src->o4*42
        - src->o3*30
        - src->o2*2
        + src->x3*60
        + src->x2*3*/;

//    if (destNode->o4 > 0 && destNode->o3 > 0) {
//            ret += 1000;
//    }
  }
#undef c3my


  destNode->rating = ret;

  unsigned long delta = GetTickCount() - beginTime;
  rateTime += delta;

};

//==================================================================

//hypothetic move
short int Al_ext::put(TNodeLink *link) {
  cursorLink = link;
  TNode *dest = link->node;
  int t = 0;
  int x = link->move%fsize;
  int y = link->move/fsize;

  if (gameMode == 1 &&  link->node->age == 1) {
            for (int i=-3; i<=3; i++)
              for (int j=-3; j<=3; j++)
                if (i==-3||i==3||j==-3||j==3)
                  if (enable(7+i,7+j))
                      history[count].en[t++] = (7+j)*fsize+7+i;
  } else {
        int max = link->node->age > 1 ? 3 : 2;
        for (int d=1; d<max; d++)
            for (int i=-1; i<2; i++)
              for (int j=-1; j<2; j++)
                if ((!i)&&(!j)) continue;
                else if (enable( x+i*d,y+j*d))
                history[count].en[t++] = (y+j*d)*fsize+x+i*d;
  }

  while (t<16) history[count].en[t++] = 255;

#ifdef DEBUG_VER
  if (kl[link->move] >= 2) {
        //should never happen
        kl[link->move] = kl[link->move]/0;
  }
#endif

  kl[link->move] = (id+1)<<2;
  history[count].nm = link->move;
  if (count == 0) {
          history[count].symmX = 0;
          history[count].symmY = 0;
          history[count].symmXY = 0;
          history[count].symmW = 0;
          history[count].symmXW = 0;
          history[count].symmYW = 0;
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
  history[count].node = dest;
  history[count].link = link;
  ++count;
  id = !id;
};

//==================================================================


//handler of user's move
int Al_ext::put(int N0, bool& swapX, bool& swapY, bool& swapW) {
  if (kl[N0] > 3) {
    return -99999;
  }

  if (!allow(N0)) {
    return -99999;
  }

  bool notfound = true;
  int N = N0;
  int x = N % 15, y = N / 15, attempt = 0;
  TNodeLink *link;
  //bool swX = swapX, swY = swapY, swW = swapW, swXYW = swapXYW;
  bool swX = false, swY = false, swW = false;

  do {
    for(int i=0; i<lastmove->totalDirectChilds; ++i) {
      link = &(lastmove->childs[i]);
      if (link->move == N) {
            lastmove = link->node;
            notfound = false;
            break;
      }
    }
    if (notfound) {
        N = N0;
        int old = N;
                if (attempt < 1 && history[count-1].symmX  == 0 && x!= 7) {
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
                }

        if (N == old) {
            TNodeLink* newChilds = new TNodeLink[lastmove-> totalDirectChilds+1];
            for(int i = 0; i < lastmove->totalDirectChilds; ++i) {
                newChilds[i] = lastmove->childs[i];
            }
            link = new TNodeLink();
            link->move = N;
            link->node = new TNode();
            newChilds[lastmove->totalDirectChilds] = *link;
            delete[] lastmove->childs;
            lastmove->childs = newChilds;
            ++lastmove->totalDirectChilds;
            ++lastmove->totalChilds;
            link->node->age = lastmove->age + 1;
            link->node->firstParent = lastmove;
            movesHash->put(link->node);
            rate(lastmove, link);
        }
    }
  } while (notfound);
  swapX = swapX ^= swW ? swY : swX;
  swapY = swapY ^= swW ? swX : swY;
  swapW = swapW ^= swW;

  put(link);
  //assert cursor==lastmove;
  return lastmove->rating;
};

//==================================================================

void Al_ext::takeback() {
  id = !id;
  --count;
  cursorLink = (TNodeLink*)history[count-1].link;
  kl[history[count].nm] = 1;
  for(int t=0; t<15; t++)
    if (history[count].en[t] != 255)
        kl[history[count].en[t]] = 0;
    else break;
};

void Al_ext::restart() {
  while(count > 1) {
     takeback();
  }
  lastmove = first->node;
}
//==================================================================

TByte coord(int i, int j) {
  return i+7+(j+7)*15;
};

//==================================================================

int Al_ext::refineList(TByte *buf, int childCount) {
        if (history[count-1].symmX && history[count-1].symmY && history[count-1].symmXY && history[count-1].symmW
                //&& history[count-1].symmXW && history[count-1].symmYW && history[count-1].symmXYW
                ) {
                return childCount;
        }

        int filteredCount = 0;
        for(int i = 0; i < childCount; ++i) {
                int x = buf[i]%fsize - 7;
                int y = buf[i]/fsize - 7;

                bool filterout = history[count-1].symmX  == 0 && x < 0
                              || history[count-1].symmY  == 0 && y < 0
                              //|| history[count-1].symmXY == 0 && x + y < 0
                              || history[count-1].symmW == 0 && x < y
                              //|| history[count-1].symmXW  == 0 && (x<0 && y>0 || x>0 && y<0)
                              //|| history[count-1].symmYW == 0 && (x<0 && y>0 || x>0 && y<0)
                              || history[count-1].symmXYW == 0 && x + y  < 0
                              ||!allow(buf[i]);
                if (filterout) {
                        filteredCount++;
                } else if (filteredCount > 0) {
                        buf[i-filteredCount] = buf[i];
                }
        }
        return childCount - filteredCount;
}

//==================================================================

int Al_ext::getList(TNode *node, TByte *buf) {
#ifdef DEBUG_VER
  if (count == 1) {
      buf[0] = coord(1,0);
      buf[1] = coord(1,1);
      buf[2] = coord(2,0);
      buf[3] = coord(2,1);
      buf[4] = coord(2,2);

      buf[5] = coord(3,0);
      buf[6] = coord(3,1);
      buf[7] = coord(3,2);
      buf[8] = coord(3,3);

      buf[9] = coord(4,0);
      buf[10] = coord(4,1);
      buf[11] = coord(4,2);
      buf[12] = coord(4,3);
      buf[13] = coord(4,4);

      return 14;
  }
#endif
  int nbuf = 0;
  if (node->x4 > 0) {
    nbuf = makelist(0, buf);//построить 5
    if (!nbuf) {
        node->x4 = 0;
    }
  }
  if (!nbuf && node->o4) {
    nbuf = makelist(1, buf);//закрыть 4
    if (!nbuf) {
        node->o4 = 0;
    }
  }
  if (!nbuf && node->x3) {
    nbuf = makelist(2, buf);//построить откр 4
    if (!nbuf) {
        node->x3 = 0;
    }
  }
  if (!nbuf && node->o3) {
    nbuf = makelist(7, buf);//7 means 3&4 - постр. 4 или закрыть 3
    if (!nbuf) {
        node->o3 = 0;
    }
  }
  if (!nbuf) {
    nbuf = makelist(-1, buf);//any move
  }
  return nbuf;
};

//==================================================================

#ifdef DEBUG_VER
bool Al_ext::verify(TNode *node, TByte move) {
        TNode *n = node;
        do {
                TNodeLink *l = n->getLink(first);
                if (l == NULL) {
                        //should never happen
                        return false;
                }
                TByte m = l->move;
                if (m != move) {
                        bool found = false;
                        for(int i=0; i<count; ++i) {
                                if ((((TNodeLink*)(history[i].link))->move) == m) {
                                        found = true;
                                        break;
                                }
                        }
                        if (!found) {
                        //should never happen
                                return false;
                        }
                }
                n = n->firstParent;
        } while (n != NULL);
        return true;
}
#endif

//==================================================================
//adds childs to cursor node
void Al_ext::expand() {
  TNode* cursor = cursorLink->node;
  if (cursor->rating >= 32300 || cursor->rating <= -32300) {
        return;
  }
  TMove buf[maxvar];
  int directChildsCount = getList(cursor, buf);
  directChildsCount = refineList(buf, directChildsCount);

  if (directChildsCount <= 0 || directChildsCount > 200) {
        //never happens??
        ++cursor->totalChilds;
        updatedParentsCounter = 0;
        updateParents(cursor, 1, cursor->rating);
        return;
  } else {
        if (cursor->totalDirectChilds != 0) {
                //should never happen
                cursor->totalDirectChilds = 0;
        }
        cursor->totalDirectChilds = directChildsCount;
  }
  short int max_rating = -32600;

  cursor->childs = new TNodeLink[directChildsCount];
  int addChilds = 0;

  for(int i=0; i<directChildsCount; ++i) {

#ifdef DEBUG_VER
    if (kl[buf[i]] >= 2) {
        //should never happen
        directChildsCount = directChildsCount;
        continue;
    }
#endif

    THash hashCodeX = cursor->hashCodeO;
    THash hashCodeO = cursor->hashCodeX * simplyGen->getHash(buf[i]);

#ifdef DEBUG_VER
    if (hashCodeO == 0) {
        hashCodeO = 4;
    }
#endif

    TNodeLink *link = &(cursor->childs[i]);

    link->node = movesHash->get(hashCodeX, hashCodeO, cursor->age + 1);

#ifdef DEBUG_VER
    if (link->node != NULL && !link->node->fixedRating && !verify(link->node, buf[i])) {
       link->node = NULL;
       ++movesHash->miss4;
    }
#endif

    if (link->node == NULL) {
#ifdef DEBUG_VER
        link->isAttached = false;
#endif
        link->node = new TNode();
        link->node->age = cursor->age + 1;
        link->node->firstParent = cursor;

#ifdef DEBUG_VER
        if (cursor->totalDirectChilds == 0) {
                //should never happen
                cursor->totalDirectChilds = cursor->totalDirectChilds;
        }
#endif
        link->move = buf[i];
        rate(cursor, link);
        movesHash->put(link->node);
    } else {
        if (link->node->firstParent == NULL) {
                link->node->firstParent = cursor;
        } else {
                TNodeLink2** next = &(link->node->nextParent);
                while((*next) != NULL) {
                        next = &((*next)->next);
                }
                next[0] = new TNodeLink2();
                next[0]->node = cursor;
                addChilds += link->node->totalChilds;
        }
#ifdef DEBUG_VER
        link->isAttached = true;
#endif
        link->move = buf[i];
    }
    if (link->node->rating > max_rating) max_rating = link->node->rating;
  }
  short int oldRating = cursor->rating;
  if (!cursor->fixedRating) {
        cursor->rating = (TRating)(0.4*(double)cursor->rating-0.6*(double)max_rating);
  }

  cursor->totalChilds = directChildsCount + addChilds;

  updatedParentsCounter = 0;
  updateParents(cursor, cursor->totalChilds, oldRating);

  if (count > max_count) max_count = count;
};

//subroutine for updateParents(), to be called for each child
void Al_ext::updateNode(TNode *node, int childsAdded) {
    int add = childsAdded;
    for(int i=0; i<updatedParentsCounter; ++i) {
        if (node == updatedParents[i]) {
                add = 0;
                break;
        }
    }
    short int old = node->rating;
    short int newRating = -32600;
    if (node->totalDirectChilds == 0 || node->fixedRating) {
        newRating = node->rating;
    } else {
        newRating = -32600;
        for(int i=0; i<node->totalDirectChilds; ++i) {
          TNode *nod = node->childs[i].node;
          short int r = nod->rating;
          if (r > newRating) {
                newRating = r;
          }
        }
        newRating = -newRating;
    }
    bool upd = (newRating != node->rating);
    if (upd || add != 0) {
            node->update(newRating, add);
            updatedParents[updatedParentsCounter++] = node;
            if (updatedParentsCounter > MAX_UPDATED_PARENTS) {
                //should never happen
                    updatedParentsCounter = MAX_UPDATED_PARENTS;
            }
            updateParents(node, childsAdded, old);
    }
}

//updates rating and child totals for all parents for given node
//childsAdded - how many childs was added to node
//oldRating - rating of this node that is used to be before it is updated
void Al_ext::updateParents(TNode *node, int childsAdded, short int oldRating) {

    if (node->firstParent == NULL) {
        if (node->rating > oldRating) {
            oldRating = oldRating;//increase
        } else if (node->rating < oldRating) {
            oldRating = oldRating;//decrease
        }
    } else {
        updateNode(node->firstParent, childsAdded);
        TNodeLink2* next = node->nextParent;
        while(next != NULL) {
            updateNode(next->node, childsAdded);
            next = next->next;
        }
    }
}

//==================================================================

void Al_ext::recalc_rating(int count) {
  TNode* node = (TNode*)history[count-1].node;
  short int r_max = -32600;
  int depth = 0;
  for(int i=0; i<node->totalDirectChilds; ++i) {
    TNode *nod = node->childs[i].node;
    short int r = nod->rating;
    if (nod->totalChilds > 0 || !depth || r > 10000) {
        if (nod->totalChilds > 0) {
            depth = 1;
        }
        if (r_max < r) r_max = r;
    }
  }
  r_max = - r_max;


  if (r_max != node->rating) {
    //bool grow = r_max > node->rating;

    if (count == 2 && first->node->totalChilds > 100000) {
        int diff = node->rating - r_max;
        //diff = diff*diff;
        if (diff<0) diff=-diff;
        recalc_deviation = (recalc_deviation_count*recalc_deviation+diff)/(recalc_deviation_count+1);
        ++recalc_deviation_count;
    }

    node->rating = r_max;
    if (count > 2) recalc_rating(count-1);
  }
}

//==================================================================

TNodeLink *Al_ext::chooseNodeToExpand() {
  TNode *cursor = cursorLink->node;
  float f0 = -99999;
  float f01 = -99999;
  TNodeLink *choosen = NULL;
  TNodeLink *choosen1 = NULL;
  TNodeLink *choosen1prev = NULL;
  do {
          for (int i = 0; i < cursor->totalDirectChilds; ++i) {
            TNodeLink *link = &(cursor->childs[i]);
            TNode *node = link->node;
            int ttc = node->ratingToTotalChilds();
            int ret = node->totalChilds > 60000 ? ttc*ttc : ttc;
            float f = ret/ (float)(1+node->totalChilds);
//            if (node->totalChilds > 500000) {
//                f = f*ttc;
//            }
            if (f > f0 || f0 == -99999) {
              choosen = link;
              f0 = f;
            }
            if (ret > f01) {
                choosen1prev = choosen1;
                choosen1 = link;
                f01 = ret;
            }
          }
//          if (choosen1->node->rating < 10000 && choosen1->node->rating > -10000 && !(first->node->totalChilds % 5)) {
//                        if (choosen1prev != NULL && choosen1prev->node->rating < 10000 && choosen1prev->node->rating > -10000) {
//                                double f1prev = choosen1prev->node->ratingToTotalChilds(first->node) / (double)(1+choosen1prev->node->totalChilds);
//                                double f1 = choosen1->node->ratingToTotalChilds(first->node) / (double)(1+choosen1->node->totalChilds);
//                                choosen = f1 >= f1prev ? choosen1 : choosen1prev;
//                        } else {
//                                choosen = choosen1;
//                        }
//          }
          if (cursorLink->node != lastmove) {
                if (choosen->node->rating >= 32300 || choosen->node->rating <= -32300) {
                    //bad situation
                    //we add 10 more fake childs to such node, so it does not want to expand more..
                    if (lastmove->rating < 32300 && lastmove->rating > -32300) {
                            updatedParentsCounter = 0;
                            updateNode(choosen->node, 10);
                    }
                    continue;
                }
          }
  }
  while(false);

  return choosen;
};

//==================================================================

void Al_ext::buildTree() {
  //assert cursor==lastmove;
  count0=count;

  while(cursorLink->node->totalDirectChilds > 0 && count < 224) {
    TNodeLink *link = chooseNodeToExpand();
    put(link);
  }

  expand();

  TNode *expanded = cursorLink->node;
  while(count>count0) takeback();

  int crit = first->node->rating*2;
  if (crit < 0) {
        crit = -crit;
  }

  /*
  //Auto cleaning (not works)
  if (crit > 3300 && (expanded->rating < -crit || expanded->rating > crit)) {
        while (expanded->firstParent->rating < -crit || expanded->firstParent->rating > crit) {
                expanded = expanded->firstParent;
        }
        expanded->cleanChilds(crit, 0);
  }
  */
  //assert cursor==lastmove;
}


//==================================================================

int Al_ext::move() {
 int f0 = -99999;
  TNode *choosen = 0;
  TNodeLink *choosenLink = 0;
  int totl = cursorLink->node->totalDirectChilds;
  if (totl == 0) {
        return -32600;
  }

  for (int i = 0; i < totl; ++i) {
    TNodeLink *link = &(cursorLink->node->childs[i]);
    TNode *node = link->node;
    int f = node->chooseFactor(node);
    if (f>f0 || f0 == -99999) {
      choosen = node;
      choosenLink = link;
      f0=f;
    }
  }
  if (choosen == NULL) {
  //should never happen??
        return -32600;
  }

  lastmove=choosen;
  put(choosenLink);
  return choosen->rating;
};

//==================================================================

int Al_ext::cleanTree(TNode *node) {
  return first->node->cleanChilds(13000, 0);
}


//==================================================================

//initial implementation for supporting of prohibited moves for
//balanced games, currently supported 1st player
//to make 2nd move in central 5x5 square
bool Al_ext::allow(int move) {
  if (gameMode == 1 && this->cursorLink->node->age == 1) {
        int x = move % 15 - 7, y = move / 15 - 7;
        return ! (x < 3 && x > -3 && y < 3 && y > -3);
  }
  return true;
}

