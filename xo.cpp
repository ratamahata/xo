//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("xomain.cpp", MainForm);
USEFORM("debug.cpp", DebugForm);
USEFORM("setup.cpp", SetupForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        Application->Initialize();
        Application->Title = "X & O game";
        Application->CreateForm(__classid(TSetupForm), &SetupForm);
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TDebugForm), &DebugForm);
                 Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
