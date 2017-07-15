//---------------------------------------------------------------------------
#ifndef debugH
#define debugH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "xo_al_ext.h"
#include "xomain.h"
#include <ComCtrls.hpp>
#include "cspin.h"
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TDebugForm : public TForm
{
__published:	// IDE-managed Components
    TMemo *Memo1;
    TButton *Build;
    TTreeView *TreeView1;
    TCSpinEdit *CSpinEdit1;
    TButton *Visualise;
    TCSpinEdit *CSpinEdit2;
    TLabel *Label1;
    TLabel *Label2;
        TCheckBox *CheckBoxLM;
        TButton *ButtonClean;
        TButton *Check;
    void __fastcall BuildClick(TObject *Sender);
    void __fastcall VisualiseClick(TObject *Sender);
        void __fastcall ButtonCleanClick(TObject *Sender);
        void __fastcall CheckClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TDebugForm(TComponent* Owner);
    void output();
    void treeOutput(TNode *link, int depth);
    void tostr(TNode *link, char* buf);
    TNode *getRoot();
    GameBoard *tree;
};
//---------------------------------------------------------------------------
extern PACKAGE TDebugForm *DebugForm;
//---------------------------------------------------------------------------
#endif
