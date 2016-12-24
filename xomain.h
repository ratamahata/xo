//---------------------------------------------------------------------------
#ifndef xomainH
#define xomainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <mmsystem.h>
//---------------------------------------------------------------------------
//#include "xo_al.h"
//#include "xo_al_ext.h"
#include "gameBoard.h"
#include "setup.h"
#include "debug.h"
#include <ExtCtrls.hpp>
#include "cspin.h"
#include <ComCtrls.hpp>
#include "CSPIN.h"
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TDrawGrid *grid;
    TPanel *Panel1;
    TButton *ButtonTB;
    TButton *ButtonM;
    TButton *ButtonR;
    TStatusBar *StatusBar;
    TComboBox *DBank;
    TRadioButton *RadioButton1;
    TRadioButton *RadioButton2;
    TButton *ButtonA;
        TCheckBox *CheckBoxGrow;
    TLabel *Label1;
        TCheckBox *CheckBoxSH;
        TButton *Button1;
        TLabel *Label2;
        TLabel *Label3;
        TComboBox *ComboBoxMode;
        TLabel *Label4;
        TLabel *Label5;
        TCSpinEdit *CSpinEditChilds;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *LabelAverages;
        TCheckBox *cbVisualRating;
    void __fastcall FormResize(TObject *Sender);
    void __fastcall gridDrawCell(TObject *Sender, int Col, int Row,
          TRect &Rect, TGridDrawState State);
    void __fastcall gridClick(TObject *Sender);
    void __fastcall ButtonTBClick(TObject *Sender);
    void __fastcall ButtonMClick(TObject *Sender);
    void __fastcall ButtonRClick(TObject *Sender);
    void __fastcall RadioButton1Click(TObject *Sender);
    void __fastcall RadioButton2Click(TObject *Sender);
    void __fastcall ButtonAClick(TObject *Sender);

    void __fastcall gridMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations

    int transform(int move);

public:		// User declarations
//    Al_ext *xo;
    GameBoard *xo;
    int lang,//1 = Eng, 2 = Rus
        viewmode,//0 = game, 1 = about info
        time_lev;
    Graphics::TBitmap *about;
    __fastcall TMainForm(TComponent* Owner);
    void __fastcall aftermove();
    void __fastcall setoptions();
    //DWORD WINAPI __stdcall grow(LPVOID lpParam);

    bool restartRequested;
    bool takeBackRequested;
    bool exitRequested;
    bool moveRequested;
    int resultRecieved;
    int userMoveRequested;
    int movesCount;

    unsigned char dkl[fsize*fsize];
    
    char msg1[200];
    char msg2[200];
    char msg3[200];
    char msg4[200];
    char msg5[200];
    short int xRating;

    char msgStatus[200];

    bool swapX, swapY, swapW;

};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
