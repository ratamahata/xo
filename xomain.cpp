//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <shellapi.h>
#include <stdio.h>
#include <windows.h>
#include "xomain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TMainForm *MainForm;

#define MAX_CHILDS_WIZARD  40000000

#define MAX_CHILD_PER_MOVE_ZONE0  6000000
#define MAX_CHILD_PER_MOVE_ZONE1  1000000

#define MAX_CHILD_DECREASE_FACTOR 6
#define MAX_CHILD_DECREASE_SINCE 16000000
#define MAX_CHILD_DECREASE_TILL 32000000

#define ZONE01_RATING 5700
#define ZONE12_RATING 10000


DWORD WINAPI __stdcall grow(LPVOID lpParam) {
 TMainForm *f =(TMainForm*)lpParam;
 while (!f->exitRequested) {

         if (f->xo == NULL) {
          Sleep(100);
          continue;
        }


        f->xo->grow(f->ComboBoxMode->ItemIndex);
        f->Invalidate();
        f->grid->Invalidate();
 }
}

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner) {

  ComboBoxMode->ItemIndex = 1;
#ifdef DEBUG_VER
#else
        this->Button1->Visible = false;
        this->LabelAverages->Visible = false;
#endif
  exitRequested = false;
  viewmode = 0;

  lang = 1;
  time_lev = 1;
  setoptions();
  CheckBoxGrow->Checked = false;
//  CheckBoxGrow->Enabled = false;

  DWORD   dwThreadIdArray;

  HANDLE h  = CreateThread(
      NULL,                   // default security attributes
      0,                      // use default stack size
      grow,       // thread function name
      this,          // argument to thread function
      0,                      // use default creation flags
      &dwThreadIdArray);   // returns the thread identifier

}

int TMainForm::transform(int move) {
  int x = move % 15, y = move / 15;
  return xo->transform(x,y);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormResize(TObject *Sender)
{ int min = (((ClientWidth-Panel1->Width<ClientHeight - StatusBar->Height) ?
               ClientWidth-Panel1->Width:ClientHeight - StatusBar->Height))/15;
  grid->Width=min*15;
  grid->Height=min*15;
  grid->DefaultColWidth = min-1;
  grid->DefaultRowHeight= min-1;
  Panel1->Left = (grid->Width + ClientWidth - Panel1->Width)/2;
  Panel1->Top  = (ClientHeight - Panel1->Height - StatusBar->Height)/2;
  if (viewmode) viewmode = 0;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::gridDrawCell(TObject *Sender, int Col, int Row,
      TRect &Rect, TGridDrawState State) {

      
  Rect = grid->CellRect(Col,Row);
  int N = transform(Row * grid->ColCount + Col);
  bool found = false;
  bool foundLast;
  for(int i = 0; i < xo->movesCount; ++i) {
      if (xo->getMove(i)->move == N) {
          found = true;
          foundLast = i == xo->movesCount - 1;
          break;
      }
  }

  switch (viewmode)
  { case 0: {
    grid->Canvas->FillRect(Rect);
    if (grid->Width>440) {
      Rect.Right--; Rect.Bottom--;
      Rect.Left++; Rect.Top++;
    }
    float penW = 3.0;
    if (grid->Width > 450) {
        penW = (grid->Width + 148)/150;
    }
    grid->Canvas->Pen->Width = penW;
    if (found && xo->kl[N]&8) {//draw 'o'
      grid->Canvas->Pen->Color = foundLast ? (Graphics::TColor) RGB(255, 136, 136) : clRed;
      grid->Canvas->Ellipse(Rect.Left+2,Rect.Top+2,Rect.Right-2,Rect.Bottom-2);
    }
    if (found && xo->kl[N]&4) {//draw 'x'
      grid->Canvas->Pen->Color = foundLast ? (Graphics::TColor) RGB(136, 136, 255) : clBlue;
      grid->Canvas->MoveTo(Rect.Left+2,Rect.Top+2);
      grid->Canvas->LineTo(Rect.Right-3,Rect.Bottom-3);
      grid->Canvas->MoveTo(Rect.Right-3,Rect.Top+2);
      grid->Canvas->LineTo(Rect.Left+2,Rect.Bottom-3);
    }
    if (CheckBoxSH->Checked) {//draw hint
      grid->Canvas->Pen->Width = xo->dkl[N] > 250 ? penW / 1.1 : penW / 1.5;
      if (xo->dkl[N]>=1  && xo->isAlllowed(N)) {
        int d = 255 - xo->dkl[N];
        grid->Canvas->Pen->Color = (Graphics::TColor) RGB(d, 255, d);
        float zf = penW*2;
        if (xo->movesCount%2) {
                grid->Canvas->Ellipse(Rect.Left + zf, Rect.Top + zf,
                        Rect.Right - zf, Rect.Bottom - zf);
        } else {
                grid->Canvas->MoveTo(Rect.Left + zf, Rect.Top + zf);
                grid->Canvas->LineTo(Rect.Right- zf, Rect.Bottom - zf);
                grid->Canvas->MoveTo(Rect.Right- zf, Rect.Top + zf);
                grid->Canvas->LineTo(Rect.Left + zf, Rect.Bottom - zf);
        }
      }
    }
    }

    break;
    case 1:
      grid->Canvas->CopyMode = cmSrcCopy;
      grid->Canvas->CopyRect(Rect,about->Canvas,Rect);
 }
};
//---------------------------------------------------------------------------
void __fastcall TMainForm::gridClick  (TObject *Sender)
{ if (viewmode)
  { if ((grid->Row == 12)&&(grid->Col<9)&&(grid->Col>2))
       ShellExecute(NULL,NULL,"http:\\\\xogame.chat.ru",NULL,NULL,SW_SHOWDEFAULT);
    else ButtonAClick(NULL);
    return;
  }
  xo->userMoveRequested = transform(grid->Row * grid->ColCount + grid->Col);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonTBClick(TObject *Sender) {
    xo->takeBackRequested = true;/*
if (xo->count)
  { xo->takeback();
    int i;
    TGridDrawState State;
    TRect Rect;
    i = GetTickCount();
    while(GetTickCount()-i < 100);
    i = xo->history[xo->count].nm;
    setkaDrawCell(NULL, i%15, i/15, Rect, State);
    i = GetTickCount();
    while(GetTickCount()-i < 100);
    if ((CheckBoxAM->Checked)&&(xo->count)) xo->takeback();
    for (i = 0; i<225; dkl[i++]=0);
    if (xo->count)
    dkl[xo->history[xo->count-1].nm] = 4;
    setka->Invalidate();
  }*/
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ButtonMClick(TObject *Sender) {
    xo->moveRequested = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::aftermove()
{
  int res =     xo->resultRecieved;
  xo->resultRecieved = 0;
  char f[] = "o.wav\0         \0";
  if (xo->movesCount%2) f[0] = 'x';
//  Application->ProcessMessages();
  PlaySound(f,NULL,SND_FILENAME|SND_ASYNC);
  StatusBar->Panels->Items[0]->Text = (AnsiString)(xo->movesCount%2 ? 'O':'X')
                                  + DBank->Items->Strings[21 + lang];
  if (res >= 32600)
  { PlaySound("gameover.wav",NULL,SND_FILENAME|SND_ASYNC);
    Application->MessageBox("GAME OVER !!!","Message",0);
  }

  int i=1;
  char pr = ' ';
  AnsiString s = DBank->Items->Strings[25 + lang];
//  itoa((xo->ntime+50)/1000,s.c_str()+i++,10);
  for(; i< 12; i++)
    if (*(s.c_str()+i) == 0) break;
  *(s.c_str()+i++) = '.';
//  itoa((xo->ntime - ((xo->ntime+50)/1000)*1000 + 50)/100,s.c_str()+i++,10);
//  if (xo->maxdepth != xo->_maxdepth) pr = '!';
  for(; i< 15; i++)
    if (*(s.c_str()+i) == 0) *(s.c_str()+i) = pr;
//  itoa(totl, s.c_str()+30, 10);
  for(i = 31; i< 40; i++)
    if (*(s.c_str()+i) == 0) *(s.c_str()+i) = ' ';
  *(s.c_str()+47) = '??';
  if (res != -1) itoa(res,s.c_str()+47,10);
  StatusBar->Panels->Items[1]->Text = s;
  grid->Invalidate();
};

//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonRClick(TObject *Sender) {
  xo->restartRequested = true;
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::setoptions() {
  ButtonM->Caption = DBank->Items->Strings[3 + lang];
  ButtonM->Hint = DBank->Items->Strings[5 + lang];
  ButtonTB->Caption = DBank->Items->Strings[7 + lang];
  ButtonR->Caption = DBank->Items->Strings[9 + lang];
  ButtonA->Caption = DBank->Items->Strings[27 + lang];
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::RadioButton1Click(TObject *Sender)
{ lang = 2;
  setoptions();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RadioButton2Click(TObject *Sender)
{ lang = 1;
  setoptions();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ButtonAClick(TObject *Sender)
{ if (viewmode)
  { viewmode = !viewmode;
    delete about;
    grid->Invalidate();
  }
  else
  { int x;
    about = new Graphics::TBitmap;
    about->Height = x = grid->Height;
    about->Width = x;
    TCanvas *c = about->Canvas;
    c->Font = Font;
    c->Font->Height = x*0.09;
    c->TextOut(x*0.21,x*0.125,"&");
    c->Pen->Width = x/104;
    c->Pen->Color = clBlue;
    c->MoveTo(x*0.12,x*0.075);
    c->LineTo(x*0.18,x*0.192);
    c->MoveTo(x*0.18,x*0.075);
    c->LineTo(x*0.12,x*0.192);
    c->Pen->Color = clRed;
    c->Ellipse(x*0.28,x*0.075,x*0.35,x*0.193);
    c->TextOut(x*0.425 - x*x*0.00003,x*0.125,"v e r   2.1");
    c->Font->Height = x*0.06;
    c->TextOut(x*0.15,x*0.544,"R  O  M  A  N      M  A  K  A  R  O  V");
    c->Font->Height = x*0.05;
    c->TextOut(x*0.608,x*0.223,"  a u g u s t  2 0 1 6");
    c->TextOut(x*0.075,x*0.484,"w r i t t e n   b y");
    c->TextOut(x*0.2,x*0.888,"e-mail:   partorgg@gmail.com");
    c->Font->Color = RGB(80,0,150);
    c->TextOut(x*0.2,x*0.82,"http:\\\\xogame.chat.ru");
    TRect Rect;
    TGridDrawState State;
    viewmode = !viewmode;
    for(int i = 0; i<225; i++)
    { gridDrawCell(NULL, i%15, i/15, Rect, State);
      x = GetTickCount();
      while(GetTickCount()-x < 10);
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::gridMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y) {
  int sx,sy;
  grid->MouseToCell(X, Y, sx, sy);

  if (viewmode && (sy==12) && (sx<9) && (sx>2)) {
        grid->Cursor = crHandPoint;
  } else if (SetupForm->balance && xo->lastMove()->age==1 && sx>4&&sx<10&&sy>4&&sy<10) {
        grid->Cursor = crNo;
  } else {
     grid->Cursor = crDefault;
  }
}


//---------------------------------------------------------------------------

void __fastcall TMainForm::Button1Click(TObject *Sender)
{
        DebugForm->Show();
}

//---------------------------------------------------------------------------


void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
        exitRequested = true;
        Sleep(50);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
        exitRequested = true;
        Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
        exitRequested = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormPaint(TObject *Sender) {

        if (xo->resultRecieved != 0) {
                aftermove();
        }

        Label1->Caption = xo->getMsg1();
        Label2->Caption = xo->getMsg2();
        Label3->Caption = xo->getMsg3();
        Label8->Caption = xo->getMsg4();
        LabelAverages->Caption = xo->getMsg5();
        StatusBar->Panels->Items[1]->Text = xo->getMsgStatus();


        int r=0, b=0;

        if (cbVisualRating->Checked) {
                short int xRating =
                          xo->movesCount%2
                        ? xo->lastMove()->rating
                        : -xo->lastMove()->rating;

                if (xRating > 0) {
                        r = 90*xRating/32600;
                } else {
                        b = -90*xRating/32600;
                }
        }

        Graphics::TColor color = (Graphics::TColor) RGB(255-r, 255, 255-b);
        grid->Color = color;
        Panel1->Color = color;
        grid->Canvas->Brush->Color = color;
        Color = color;
}
//---------------------------------------------------------------------------





void __fastcall TMainForm::FormShow(TObject *Sender)
{
  if (CheckBoxGrow->Checked) return;
  CheckBoxGrow->Checked  = true;

  //xo = new Al_ext(SetupForm->balance);

  Logger *logger = new Logger();
  SimplyNumbers *sn = new SimplyNumbers();
  Hashtable *ht = new Hashtable(logger);

  xo = getXBoard(SetupForm->balance ? 1 : 0);

  xo->logger = logger;
  
//  xo->mindepth = 1;
  
}
//---------------------------------------------------------------------------


