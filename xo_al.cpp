//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "xo_al.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#define linec 54
unsigned char line [2][linec][9]=
{  "XXXXX","88888",//0,1
   "_XXXX_","x_XXX_x","xx_XX_xx","xxx_X_xxx",//2
//  3: close opened 3
   "_8888_","Xooo__","__oooX","To_ooT","Too_oT","oToToTo",
   "oTTooTo","oTooTTo","oooTTTooo","*_ooo_*",
//  4: build closed 4
   "XXXX_","_XXXX","XXX_X","X_XXX","XX_XX",
//  5: build opened 3
   "_XXX__","__XXX_","_XX_X_","_X_XX_","X_X_X_X",
   "X_XX__X","X__XX_X","XX_X__XX","XX__X_XX",
//  9:  close 2 or closed 3
   "TTooo","oooTT","?oToo?","?ooTo?","oToTo","oTToo","ooTTo",
   "_TooT_","TTToo_+","+_ooTTT","ToToT_","_ToToT","_oTTo_",
//  10: build 2  or closed 3
   "_TxTT_","_TTxT_","_X__X_",
   "XXX__","__XXX","_XXX_","X_XX_","_XX_X","XX_X_","_X_XX",
//11: ToT  (for the 2nd move)
   "ToT",
};

inline bool Al::comp(int x, int y, unsigned char c)
{ if ((x<0)||(y<0)||(x>=fsize)||(y>=fsize))
    return (c&16);
  char n = kl[x+y*fsize];
  if (n<2) n = 2;
  return (n&c);
}
bool __fastcall Al::scanlines(int BlNo, int &lines, int N)
{ static const int bl[18] = {0,1,2,6,16,21,30,6,21,30,43,53,54,9,11,30,53},
     vec[4][2] = {{1,1},{1,-1},{1,0},{0,1}}, p2[4] = {1,2,4,8};
  int nvec,sdv,nline,c, x = N%fsize, y = N/fsize;
  for (nvec=0; nvec<4; nvec++) if (lines&(p2[nvec]))
    for (nline = bl[BlNo]; nline<bl[BlNo+1]; nline++)
      for (sdv=0; sdv<9; sdv++) if (line[id][nline][sdv]&32)
        for (c = 0; c<9; c++)
          if (c==sdv) goto skip; else if (!line[id][nline][c]) goto skip;
          else if (comp(x+vec[nvec][0]*(c-sdv),y+vec[nvec][1]*(c-sdv),line[id][nline][c]))
skip:     { if (c==8)
            { lines -= p2[nvec];
              return true;
          } } else break;
  return false;
};
Al::Al()
{ randomize();
  count = 0;
  id = 0;
  memset(kl,0,fsize*fsize);
  memset(history,0,255*sizeof(Thistory));
  mcache[0] = maxcache3;  mcache[1] = maxcache4;
  cache[0] = new TCache [mcache[0]];
  cache[1] = new TCache [mcache[1]];
//  memset(cache[0], 0, maxcache*maxLcache*sizeof(TCache));
  for(int i=0; i<linec*9; i++)
    switch (line[0][0][i])
    { case '_': line[0][0][i] = 2; break;//space
      case 'x': line[0][0][i] = 4; break;
      case 'o': line[0][0][i] = 8; break;
      case 'T': line[0][0][i] =34; break;//X or space
      case 'X': line[0][0][i] =36; break;//X or x
      case '8': line[0][0][i] =40; break;//X or o
      case '+': line[0][0][i] =20; break;//x or #
      case '*': line[0][0][i] =52; break;//X or x or #
      case '?': line[0][0][i] =54; break;//X or x or # or space
    };
  memcpy(line[1][0],line[0][0],linec*9);
  for(int i=0; i<linec*9; i++)
    if ((line[1][0][i]&12) != 12)
      if (line[1][0][i]&4) line[1][0][i] += 4;
      else if (line[1][0][i]&8) line[1][0][i] -= 4;
};
Al::~Al()
{ delete[] cache[0];
  delete[] cache[1];
};
//----------------------------------------------------------
//--------------------- put & takeback ---------------------
//----------------------------------------------------------

int Al::enable(int x, int y)//inline func for put
{
//  history[count].en[t++] = (y+j*d)*fsize+x+i*d;

if ((x<0)||(y<0)||(x>=fsize)||(y>=fsize)||(kl[y*fsize+x]))
    return 0;
  return kl[y*fsize+x] ? 0 : kl[y*fsize+x] = 1;
};

/*
int Al::put(int N, int& ext)
{ int x,y,t,ret = 0, _ext = ext;
  ext = 0;
  history[count+1].x3 = history[count].o3;
  history[count+1].x4 = history[count].o4;
  history[count+1].o3 = history[count].x3;
  history[count+1].o4 = history[count].x4;
  t = 15; // 'lines'
  if (_ext && history[count].x4)// построить 5
    if (scanlines(0,t,N))
    { ret = 32600;
      goto exit;
    }
#define c3my x
  c3my = false;
  if (history[count].o4)//закрыть 4
    if (scanlines(1,t,N))
      { history[count+1].x4--; ret -= 800;
        c3my = ext = 1;
      }
  y = t;
  if (history[count].x3)//постр открытую 4
    if (scanlines(2,t,N))
    { history[count+1].o4 += 2;
      history[count+1].o3 --;
      last4L = y - t;
    }
#define c3 y
  c3 = true;
  while (history[count+1].x3&&c3)//закрыть 3
    if (c3 = scanlines(3,t,N))
      { history[count+1].x3--;
        ret -= 600;
        ext = 1;
      }
#undef c3
  if(_ext) t = 15;
  y = t;
  while (scanlines(4,t,N))//построить закрытую 4
  { history[count+1].o4++;
    ext = 1;
    if (c3my) c3my = 2;
    last4L = y - t;
  }
  while (scanlines(5,t,N))//постр 3
    {history[count+1].o3++; ext = 1;}
  if ((c3my==2)&&(history[count+1].o3))
  { int i=0, n15 = 15;
    for (int N = 0; N<fsize*fsize; N++)
      if (kl[N]&3)
      {  while (scanlines(2, n15, N)) i++;
         n15 = 15;
      }
    history[count+1].o3 = (i+1)/2;
  }

exit:
  if (_ext||ext)
  { if (kl[N]>2)
      return -99999;
    x = N%fsize, y = N/fsize, t = 0;
    for (int d=1; d<3; d++)
      for (int i=-1; i<2; i++)
        for (int j=-1; j<2; j++)
          if ((!i)&&(!j)) continue;
          else if (enable(x+i*d,y+j*d))
          history[count].en[t++] = (y+j*d)*fsize+x+i*d;
    kl[N] = (id+1)<<2;
    history[count].nm = N;
    while (t<15) history[count].en[t++] = 255;
    count++;
    id = !id;
    if (ret > 32000) return ret;
  }

  if (history[count].x4) return -32550;
  if ((history[count].x3)&&(history[count].o4 == 0)) return -32500;
  if (history[count].o4 > 1) return(32500);
  if (history[count].o4 && history[count].o3)
    if (history[count].x3) return 10000;
    else return 32400;
  return history[count].o4*6000
       + history[count].o3*4100
       - history[count].x3*8600 + ret;
#undef c3my
};

void Al::takeback()
{ id = !id;
  count--;
  kl[history[count].nm] = 1;
  for(int t=0; t<15; t++)
    if (history[count].en[t] != 255)
        kl[history[count].en[t]] = 0;
};
*/
//-----------------------------------------------------
//-------------------   main Al  ----------------------
//-----------------------------------------------------
int Al::makelist(int BlNo, unsigned char *buf)
{ int sup = fsize*fsize, i=0, n15 = 15;
  for (int N = 0; N<sup; N++)
    if (kl[N]&3)
    { if (BlNo == -1) goto skip;
        if (scanlines(BlNo, n15, N))
skip:   { buf[i++] = N;
          n15 = 15;
          if (i == maxvar-1) break;
        }
    }
//  buf[i] = 255;//delimeter
  return i;
};


int Al::move()
{ unsigned char buf[maxvar];
  int sup = -1, nbuf = 0, i, j = 0, ext;
  bool bonus_only =1;
  nvar = 0; ntime = GetTickCount();
  maxdepth = _maxdepth; count0 = count;
  for (i = 0; i<maxLcache; ncache[i++] = 0);
  memset(dkl,0,255);
restart:
  if (history[count].x4)
  { nbuf = makelist(0, buf);
    if (!nbuf) {history[count].x4 = 0; goto restart;}
    sup = 32600;
  }
  else if (history[count].o4)
  { nbuf = makelist(1, buf);
    if (!nbuf) {history[count].o4 = 0; goto restart;}
  }
  else if (history[count].x3)
  { nbuf = makelist(2, buf);
    if (!nbuf) {history[count].x3 = 0; goto restart;}
    sup = 32500;
  }
  else if (!count) buf[nbuf++] = 112; //первый ход - в центр
  else do
  // список с отсеиванием ходов
  { if (count == 1)
    {   if ((history[0].nm<15)||(history[0].nm>=220)
           ||(history[0].nm%15 ==0)||(history[0].nm%15 == fsize-1))
           buf[nbuf++] = 112;
        else nbuf = makelist(11, buf);
        sup = 0;
        for(i = 0; i<nbuf; i++)
        if ((abs(buf[i] - history[0].nm) == 1)||(abs(buf[i] - history[0].nm) == fsize))
            buf[i] = 255;
    }
    else if (history[count].o3)
    { nbuf = makelist(7, buf);//7 means 3&4
      if (!nbuf) {history[count].o3 = 0; goto restart;}
    }
    else nbuf = makelist(-1, buf);
    sup = - 32760;
    for(i = 0; i< nbuf; i++)
    if (buf[i]<255)
    { int n15 = 15, bonus, j = 0;
      while (scanlines(9,n15,buf[i]))
            j += 35;
      while (scanlines(10,n15,buf[i]))
            j += 17;//((id)||(count>8)) ? 17 : 40;
      bonus = j;
      if (bonus) ext = 1; else ext = 0;
      j += put(buf[i], ext);
#ifdef DEBUG_VER
      if (bonus) dkl[buf[i]] = 1;
      if (ext) dkl[buf[i]] |= 2;
#endif
//
//      if (count>5)
      if (bonus || ext)//||(history[count-1].o3))
      { if ((j <= 32000)&&(j-bonus >= -32000))
          j = bonus + move_in(1, ext);
      }
      else if (bonus_only) buf[i] = 255;
      if (bonus || ext) takeback();
      if (ext) j +=15;
      if ((bonus_only)&&(buf[i] == 255)) continue;
      if (history[count].o3)
          while (scanlines(13,n15,buf[i]))
                j -= 35;
      if (sup < j)
      { sup = j;
        for(j = i-1; j >= 0; j--)
         buf[j] = 255;
      } else
          if (sup > j)
             buf[i] = 255;
    }     
    j = 0; bonus_only  = false;
    for(i = 0; i<nbuf; i++)
      if (buf[i]!=255) j++;
  } while (!j);
  if (!nbuf)
  return 0; //failed
#ifdef DEBUG_VER
  for(i = 0; i<nbuf; i++)
    if (buf[i] != 255) dkl[buf[i]] |= 4;
#endif
  ntime = GetTickCount() - ntime;
  do i = random(nbuf);
  while (buf[i] == 255);
  put(buf[i],j=1);
  return sup;
}

int __fastcall Al::move_in(int depth, int ext)
{ unsigned char buf[maxvar];
  int sup, nbuf, i, j, _ext, n;
  if (maxdepth > 3)
  {  if (GetTickCount() - ntime > ((7*ctime)>>3))
      maxdepth = 3;
  } else if (maxdepth == 3)
    if (GetTickCount() - ntime > ctime)
      maxdepth = 2;
  if (history[count].o4)
  { const int vec[4][2] = {{1,1},{1,-1},{1,0},{0,1}},
              _p2[15] = {1,0,1,0,2,0,1,0,3,0,1,0,2,0,1};
    j = _p2[last4L];//vec
    nbuf = history[count-1].nm;
    sup = 15;
    for(i = -4; i<5; i++)
      if ((kl[_ext = (nbuf/fsize + vec[j][1]*i)*fsize + nbuf%fsize + vec[j][0]*i]) < 4)
        if (scanlines(1,sup,_ext))
          {buf[0] = _ext; break;}
    if (i == 5)
      ntime = 0;
    nbuf = 1;
    last4L = 0;
  }
  else if (history[count].o3) nbuf = makelist(7, buf);//7 means 3&4
  else nbuf = makelist(-1, buf);
  if (nbuf == 1)
     depth--;
  sup = - 32760;
  bool cached;
  for(i = 0; i < nbuf; i++)
  {
    cached = 0;
    _ext = count-count0+1;
    if (_ext == 3)
    { for(j = 0; j<ncache[0]; j++)
        if (history[count0    ].nm == cache[_ext-3][j].m[2])
        if (history[count0 + 1].nm == cache[_ext-3][j].m[1])
        if (                buf[i] == cache[_ext-3][j].m[0])
           {cached = true; break;}
    } else if ((_ext == 4)&&(_maxdepth > 3))
      for(j = 0; j<ncache[1]; j++)
        if  ((history[count0 + 1].nm == cache[_ext-3][j].m[3])
           &&(                buf[i] == cache[_ext-3][j].m[1]))
        if (((history[count0    ].nm == cache[_ext-3][j].m[0])
           &&(history[count0 + 2].nm == cache[_ext-3][j].m[2]))
          ||((history[count0 + 2].nm == cache[_ext-3][j].m[0])
           &&(history[count0    ].nm == cache[_ext-3][j].m[2])))
           {cached = true; break;}

    if (cached) {j = cache[_ext-3][j].res; nvar ++;}
    else
    { _ext = 0;
      j = put(buf[i], _ext);
      if (_ext) j += 15;
      if ((depth<maxdepth)&&(depth<mindepth+ext+_ext-1))
      {   if ((j < 30000)&&(j > -30000))
             j = move_in(depth+1, ext+_ext);
          else nvar++;
      } else nvar++;
      if (_ext) takeback();

      _ext = count-count0+1;
      if ((_ext == 3)||((_ext == 4)&&(_maxdepth>3)))
      if (ncache[_ext-3] < mcache[_ext-3])
      { for (n = count0; n<count; n++)
          cache[_ext-3][ncache[_ext-3]].m[n-count0] = history[n].nm;
        cache[_ext-3][ncache[_ext-3]].m[count-count0] = buf[i];
        cache[_ext-3][ncache[_ext-3]++].res = -sup;
      }
    }
//    if (count-count0 == 4) nvar++;
    if (depth==1)
    { int n15 = 15;
      while (scanlines(9,n15,buf[i]))  j += 17;
      while (scanlines(10,n15,buf[i])) j += 8;
      while (scanlines(13,n15,buf[i])) j -= 17;
    }
    if (j > sup) if ((sup = j) >29000)  break;
  }
  if (depth>1)
  { if (sup > 25000) sup -= 1500;
    if (sup < -25000) sup += 700;
  }
  return(-sup);
};



