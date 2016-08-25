//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>

#pragma hdrstop

#include "debug.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TDebugForm *DebugForm;
//---------------------------------------------------------------------------
__fastcall TDebugForm::TDebugForm(TComponent* Owner)
    : TForm(Owner) {
//  Al_ext* al= new Al_ext();
//  tree = al->instance;
  tree = MainForm->xo;
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::BuildClick(TObject *Sender) {
    for (int t = 0; t < CSpinEdit1->Value; ++t) {
        tree->buildTree();
    }
    output();
//  treeOutput(tree->first);
}

void TDebugForm::output() {
  char buf[200];
  sprintf(buf, "items total: %d, lastExpanded: %d, max_count: %d",
    tree->first->node->totalChilds,
    0,//tree->lastExpanded->move,
    tree->max_count);
  Memo1->Lines->Add( buf);

  for(int i=0; i<tree->lastmove->totalDirectChilds; ++i) {
    TNodeLink link = tree->lastmove->childs[i];

    tostr(&link, buf);
    Memo1->Lines->Add( buf);
  }
}

void TDebugForm::tostr(TNodeLink *link, char* buf) {
    TNode* node = link->node;
    sprintf(buf, "%c%c[%d,%d](%d)[%d,%d] R:%d/%d (%d/%d/%d; %d/%d/%d), childs direct/total:%d / %d%c",
        (link->node->fixedRating?'-':' '),
#ifdef DEBUG_VER
        (link->isAttached?'*':' '),
#else
        ' ',
#endif
        link->move%15 - 7, link->move/15 - 7,
        tree->simplyGen->getHash(link->move),
        (unsigned long)link->node->hashCodeX, (unsigned long)link->node->hashCodeO,
        node->rating, node->ratingToTotalChilds(node),
        (int)node->x2, (int)node->x3, (int)node->x4, (int)node->o2, (int)node->o3, (int)node->o4,
        node->totalDirectChilds,
        node->totalChilds > 2000 ? node->totalChilds / 1000 : node->totalChilds,
        node->totalChilds > 2000 ? 'K' : ' ');
}

TNodeLink *TDebugForm::getRoot() {
        if (tree == NULL) {
          tree = MainForm->xo;
        }
        return CheckBoxLM->Checked ? tree->lastmove->getLink(tree->first) : tree->first;
}

void TDebugForm::treeOutput(TNodeLink *link, int depth) {
    if (depth >= CSpinEdit2->Value) {
        return;
    }

//#ifdef DEBUG_VER
    char buf[200];
    if (link == getRoot()) {
    	TreeView1->Items->Clear();
        tostr(link, buf);
        link->node->reserved = TreeView1->Items->Add(TreeView1->Selected, buf);
    }

    bool choosen[200];
    memset(choosen,0,200*sizeof(bool));
//    bool *choosen = new bool[link->node->totalDirectChilds];
    for(int j=0; j<link->node->totalDirectChilds; ++j) {
        short int max = -32000;
        int selected = -1;
        for(int i=0; i<link->node->totalDirectChilds; ++i) {
            if (!choosen[i]) {
                TNodeLink *child;
                child = &(link->node->childs[i]);
                short int r = child->node->rating;
                if (r > max) {
                    max = r;
                    selected = i;
                }
            }
        }
        if (selected == -1) {
        selected=selected;
            break;
        }
        choosen[selected] = true;
        TNodeLink *chosenChild = &(link->node->childs[selected]);
        tostr(chosenChild, buf);
        chosenChild->node->reserved = TreeView1->Items->AddChild((TTreeNode*)link->node->reserved, buf);
        treeOutput(chosenChild, depth+1);
    }
//#endif
//    delete[] choosen;

    /*
    //Adds a root node
	TreeView1->Items->Add(TreeView1->Selected, "RootTreeNode1");

	//Sets MyTreeNode to first node in
	//tree view and adds a child node to it
	TTreeNode *MyTreeNode1 = TreeView1->Items->Item[0];
	TreeView1->Items->AddChild(MyTreeNode1,"ChildNode1");

	//Adds a root node
	TreeView1->Items->Add(TreeView1->Selected, "RootTreeNode2");

	//Sets MyTreeNode to third node in
	//tree view and adds a child node to it
	TTreeNode *MyTreeNode2 = TreeView1->Items->Item[2];

    TreeView1->Items->AddChild(MyTreeNode2,"ChildNode2");

	//Sets MyTreeNode to fourth node in
	//tree view and adds a child node to it
	MyTreeNode2=TreeView1->Items->Item[3];
	TreeView1->Items->AddChild(MyTreeNode2,"ChildNode2a");

	//Sets MyTreeNode to fifth node in
	//tree view and adds a child node to it
	MyTreeNode2 = TreeView1->Items->Item[4];
	TreeView1->Items->Add(MyTreeNode2,"ChildNode2b");

	//add another root node
	TreeView1->Items->Add(TreeView1->Selected, "RootTreeNode3");*/
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::VisualiseClick(TObject *Sender) {
    treeOutput(getRoot(), 1);

}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::ButtonCleanClick(TObject *Sender)
{
    if (tree == NULL) {
      tree = MainForm->xo;
    }

    int count = tree->cleanTree(tree->lastmove);
    char s[100] = "      cleaned !!!";
    itoa(count, s, 10);
    Application->MessageBox(s, "Message", 0);
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::CheckClick(TObject *Sender)
{
    if (tree == NULL) {
      tree = MainForm->xo;
    }

    int count = tree->first->node->checkChilds(0);
    char s[100] = "      violations !!!";
    itoa(count, s, 10);
    Application->MessageBox(s, "Message", 0);

}
//---------------------------------------------------------------------------

