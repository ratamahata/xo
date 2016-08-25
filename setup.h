//---------------------------------------------------------------------------

#ifndef setupH
#define setupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "xomain.h"
//---------------------------------------------------------------------------
class TSetupForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TButton *Button1;
        TRadioButton *RadioButton3;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TSetupForm(TComponent* Owner);
        bool balance;
};
//---------------------------------------------------------------------------
extern PACKAGE TSetupForm *SetupForm;
//---------------------------------------------------------------------------
#endif
