#include "Gf3ROOT.h"

//defines shortcuts of Gf3ROOT functions
void Gf3ROOT::ExecuteShortCut(Int_t event,Int_t px,Int_t py,TObject* sel)
{
  if( event == 51 )
    return;
  //std::cout << "event = " << event << ", px = " << px << ", py = " << py << ", sel = " <<  sel->GetName();
  if(event==kWheelDown)
    hn();
  else if(event == kWheelUp)
    hb();
  else if(event == kButton1Shift){ //shift+click
    //TCanvas *c = (TCanvas *) gTQSender;
    //TPad *pad = (TPad *) c->GetSelectedPad();
    //TPad *pad = (TPad *) c->GetSelectedPad();
    if(gPad){
      Float_t x = gPad->AbsPixeltoX(px);
      Float_t y = gPad->AbsPixeltoY(py);
      x = gPad->PadtoX(x);
      y = gPad->PadtoY(y);
      Expand(x);
    }
  }
  else if(event == kKeyPress){
    if(px == 104) // h
      fb();
    if(px == 108) // l
      fn();
    if(px == 107) // k
      hb();
    if(px == 106) // j
      hn();
    if(px == 6 && !mode) //ctr+f
      nf();
    if(px == 12)  //ctr+l
      nf("L");
    if(px == 17)  //ctr+q
      qf();
    if(px == 21)  //ctr+u
      unzoom();
    if(px == 16)  //ctr+p
      ps();
    if(px == 13)  //ctr+m
      mf();
    if(px == 5)  //ctr+e
      esc();
    //fTCanvas->Update();
  }

  //std::cout << " exit" << std::endl;
}

