#include "GAROOT/src/GPeakSearch.h"

GPeakSearch::GPeakSearch(void) : TSpectrum()
{
  niter = 10;
  sigma = 2;
  threshold = 0.05;
}

GPeakSearch::~GPeakSearch(void){}

Int_t GPeakSearch::PSearch(TH1* HistPtr)
{
  fTH1 = (TH1F*)HistPtr->Clone();
  fTH1BG = (TH1F*)Background(fTH1,niter);
  TH1F* fTH1P = (TH1F*)fTH1->Clone();
  fTH1P->Add(fTH1BG,-1);
  return Search(fTH1P,sigma,"",threshold);
}

void GPeakSearch::SetParam(Double_t thresh, Int_t nit, Double_t sig)
{
  if(thresh < 0)
    thresh = threshold;
  if(nit < 0)
    nit = niter;
  if(sig < 0)
    sig = sigma;
  threshold = thresh;
  niter = nit;
  sigma = sig;
  return;
}

void GPeakSearch::GetPMarker(TObjArray* marker_array) const
{
  Int_t mkn = GetNPeaks();
  Double_t* posXF = GetPositionX();
  Double_t* posX = new Double_t[mkn];
  for(int i=0; i<mkn; i++){
    posX[i] = (Double_t)posXF[i];
  }
  AOrder O(mkn);
  O.Ascend(posX);
  marker_array->Clear();
  for(int i=0; i<mkn; i++){
    TMarker* marker = new TMarker();
    marker->SetMarkerStyle( gPSMarker.GetMarkerStyle() );
    marker->SetMarkerColor( gPSMarker.GetMarkerColor() );
    marker->SetMarkerSize( gPSMarker.GetMarkerSize() );
    marker->SetX((Double_t)posX[i]);
    marker->SetY(fTH1->GetBinContent(fTH1->FindBin((Double_t)posX[i])));
    marker_array->Add(marker);
  }
  return;
}
