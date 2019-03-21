#include "GAROOT/src/MkMouse.h"

MkMouse::MkMouse(void)
{
  fMarkerArray = new TObjArray();
  fMarkerArray->SetOwner();
}

MkMouse::~MkMouse(void){
  delete fMarkerArray;
}

void MkMouse::SetMarker(Int_t event, Int_t px, Int_t py, TObject *sel)
{
   TCanvas *c = (TCanvas *) gTQSender;
   TPad *pad = (TPad *) c->GetSelectedPad();
   if (event == kButton1Down) {
      Float_t x = pad->AbsPixeltoX(px);
      Float_t y = pad->AbsPixeltoY(py);
      x = pad->PadtoX(x);
      y = pad->PadtoY(y);
      TMarker* marker = new TMarker(0,0,23);
      marker->SetX(x);
      marker->SetY(y);
      marker->SetMarkerColor(fMarkerArray->GetEntries()+2);
      marker->Draw("");
      fMarkerArray->Add(marker);
   }
   return;
}

void MkMouse::Clear(void)
{
  fMarkerArray->Clear();
}

