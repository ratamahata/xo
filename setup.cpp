//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "setup.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSetupForm *SetupForm;
//---------------------------------------------------------------------------
__fastcall TSetupForm::TSetupForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSetupForm::Button1Click(TObject *Sender)
{
  if (this->RadioButton1->Checked) balance = false;
  if (this->RadioButton2->Checked) balance = true;
  if (this->RadioButton1->Checked || this->RadioButton2->Checked) {
        MainForm->Show();
        this->Hide();
  }
}

