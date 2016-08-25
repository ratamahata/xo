//---------------------------------------------------------------------------
#ifndef xo_alH
#define xo_alH
//---------------------------------------------------------------------------
#include <mem.h>
#include <stdlib.h>
#define fsize 15 //field size
#define maxvar 125  //move & move_in
#define maxcache3 2048
#define maxcache4 8190
#define maxLcache 2
struct Thistory
{ unsigned char en[20], nm, x3,x4, o3,o4;
  void *node;
  void *link;
  int symmX, symmY, symmXY, symmW, symmXW, symmYW, symmXYW;
};
struct TCache
{ unsigned char m[4];//maxLcache+2];
  int res;
};

class Al
{
  public:
  int enable(int,int);
  unsigned char kl[fsize*fsize], // 0 == empty, disabled
                                 // 1 == empty, enabled
       id;                       // 0 == 'x',first
  Thistory history[255];
  TCache* cache[maxLcache];
  int count, count0, mindepth, maxdepth, //internal vars
      _maxdepth,//external
      ntime, ctime, nvar, //indicators, etc
      last4L,//cache for put
      ncache[maxLcache],//for move_in
      mcache[maxLcache];//maximum cache
  Al();
  ~Al();
  inline bool comp(int x, int y, unsigned char c);
  bool __fastcall scanlines(int BlNo, int &lines, int N);
  void takeback();
  int put(int N, int& ext);  //player's move, 1 == wins
//  int eval(int N); //evaluate position
  int makelist(int BlNo, unsigned char *buf);
  int move();    //al's move
  int __fastcall move_in(int, int);    //al's move

  unsigned char dkl[fsize*fsize];

};
#endif
/* blocks of lines used:
  0: XXXXX - build 5
  1: 88888 - close 4
  2: build opened 4
  3: close opened 3
  4: build closed 4
  5: build opened 3
  6: delimeter I
  7: '3' and '4'
  8: delimeter I
  9:  close 2
  10: build 2
  11: ToT  (for the 2nd move)
  12: delimeter II
  13: check for bad (3)
  15: delimeter M
*/

/*//not included function
int Al::eval(int N)
{ int n15 = 15,ret = 0,
    x3 = history[count].x3, x4 = history[count].x4,
    o3 = history[count].o3, o4 = history[count].o4;
  if (x4)// построить 5
    if (scanlines(0,n15,N))
      return 32600;
  if (o4)//закрыть 4
    if (scanlines(1,n15,N))
      {o4--; ret -= 800;}
  if (x3)//постр открытую 4
    if (scanlines(2,n15,N))
    { x4 += 2;
      x3 --;
    }
  bool c3 = true;
  while (o3&&c3)//закрыть 3
     if (c3 = scanlines(3,n15,N))
      { o3--;
        ret -= 600;
      }
  while (scanlines(4,n15,N)) x4++;//построить закрытую 4
  while (scanlines(5,n15,N)) x3++;//постр 3
  if (o3) return -32550;
  if ((o3)&&(x4 == 0)) return -32500;
  if (((x4)&&(x3))||(x4 > 1)) return(32500);
  return x4*8000 + x3*4010 - o4*32500 - o3*8000 + ret;
};*/
