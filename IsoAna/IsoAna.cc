#include "IsoAna/IsoAna.h"

IsoAna::IsoAna(TH2F* ETin){
  fETmat = ETin;
  Init();
}

IsoAna::~IsoAna(void){

}

// Initialize arrays.
void IsoAna::Init(void) {
  fHist = NULL;
  hl_col = kCyan + 1;
  hf_col = kCyan + 1;
  fMarkerArray = new TObjArray();
  for(int i=0; i<kNMarkers; i++){
    TMarker *marker = new TMarker(0,0,23);
    marker->SetMarkerColor(i+2);
    fMarkerArray->Add(marker);
  }
  fFitBaseLine = new TF1("base","pol0(0)",0,30000);
  fFitBaseLine->SetLineColor(4);
  fFitBaseLine->SetLineStyle(3);
  canv = new TCanvas("canv","canv",0,0,640,640/1.58);
  fETmat->Draw("colz");
  fHistArray = new TObjArray();
  fFitArray = new TObjArray();
  fPeakRangeArray = new TObjArray();
  fComptonRangeArray = new TObjArray();

  return;
}

// Draw markers on the current histogram.
void IsoAna::dmk(void){
  if(fHistArray->IsEmpty()){
    std::cout << "No TH1 has been created." << std::endl;
    return;
  }
  else{
    TIter marker_next( fMarkerArray );
    while( TMarker *marker = (TMarker*)marker_next() ){
      marker->SetX(fHist->GetBinCenter(fHist->GetXaxis()->GetFirst()));
      marker->SetY(fHist->GetMinimum());
      marker->Draw("");
    }
  }
  return;
}

// Show positions of markers.
void IsoAna::mkc(void) const {
  std::cout << "X\tY" << std::endl;
  TIter marker_next( fMarkerArray );
  while( TMarker *marker = (TMarker*)marker_next() ){
    std::cout << marker->GetX() << "\t" << marker->GetY() << std::endl;
  }
}

// Draw current histogram
void IsoAna::draw() const {
  fHist->SetLineColor(hl_col);
  fHist->SetFillColor(hf_col);
  fHist->Draw("");
}

// Set histogram color.
void IsoAna::sethcol(Color_t lcol, Color_t fcol){
  hl_col = lcol;
  hf_col = fcol;
  draw();
}

// Set an energy range as a Compton scatter region.
void IsoAna::setct(Double_t ctlow, Double_t ctup){
  if( (ctlow==0) && (ctup==0) ){
    ctlow = ((TMarker*)fMarkerArray->At(kNMarkers-2))->GetX();
    ctup = ((TMarker*)fMarkerArray->At(kNMarkers-1))->GetX();
  }
  fComptonRangeArray->Add(new Range(ctlow,ctup));
  std::cout << "The following range has been added as a Compton part:" << std::endl;
  std::cout << ctlow << "\t" << ctup << std::endl;
  return;
}

// Set an energy range as a peak region.
void IsoAna::setpk(Double_t pklow, Double_t pkup){
  if( (pklow == 0) && (pkup == 0) ){
    pklow = ((TMarker*)fMarkerArray->At(kNMarkers-2))->GetX();
    pkup = ((TMarker*)fMarkerArray->At(kNMarkers-1))->GetX();
  }
  fPeakRangeArray->Add(new Range(pklow,pkup));
  std::cout << "The following range has been added as a peak:" << std::endl;
  std::cout << pklow << "\t" << pkup << std::endl;
  return;
}

// Make a projection on to the energy axis.
TH1D* IsoAna::prE(Double_t tlow, Double_t tup){
  ostringstream oss("");
  oss << "hist" << fHistArray->GetLast()+1;
  std::cout << oss.str() <<"has been created." << std::endl;
  fHist = (TH1D*)fETmat->ProjectionX(
      oss.str().c_str(),
      fETmat->GetYaxis()->FindBin(tlow),
      fETmat->GetYaxis()->FindBin(tup)
    ); fHistArray->Add(fHist);
  fHist->SetXTitle("Energy (keV)");
  fHist->SetYTitle("Count");
  draw();
  return fHist;
}

// Make a projection on to time axis.
TH1D* IsoAna::prT(void){
  TH1D* h_tmp = NULL;
  if( fPeakRangeArray->GetLast()<0 ){
    std::cout << "please set the ranges of peaks ahead." << std::endl;
    return NULL;
  }
  else{
    ostringstream oss("");
    oss << "hist" << fHistArray->GetLast()+1;
    std::cout << oss.str() <<"has been created." << std::endl;
    vector<Double_t> errors;
    {
      TIter peak_next( fPeakRangeArray );
      {
        Range* peak = (Range*)peak_next();
        h_tmp = fETmat->ProjectionY(oss.str().c_str(),fHist->FindBin(peak->GetMin()),fHist->FindBin(peak->GetMax()));
      }
      for(int k=0; k<h_tmp->GetNbinsX(); k++){
        const Double_t cnt = (Double_t)h_tmp->GetBinContent(k);
        errors.push_back(TMath::Sqrt(cnt));
      }
      while( Range* peak = (Range*)peak_next() ){
        TH1D* prtmp = (TH1D*)fETmat->ProjectionY("tmp",fHist->FindBin(peak->GetMin()),fHist->FindBin(peak->GetMax()));
        h_tmp->Add(prtmp);
        Int_t i = 0;
        for(auto err : errors){
          Double_t cnt = prtmp->GetBinContent(i);
          Double_t prEr = sqrt(cnt);
          err = sqrt(err*err + prEr*prEr);
          ++i;
        }
      }
    }
    TH1D* htmp2 = (TH1D*)h_tmp->Clone();
    TIter compton_next( fComptonRangeArray );
    while( Range* compton = (Range*)compton_next() ){
      Double_t coef = 0;
      TIter peak_next( fPeakRangeArray );
      while( Range* peak = (Range*)peak_next() )
	coef += peak->GetWidth();
      coef = -1.0*coef / ( compton->GetWidth() );
      std::cout << coef << std::endl;
      TH1D *prtmp = (TH1D*)fETmat->ProjectionY("tmp",fHist->FindBin(compton->GetMin()),fHist->FindBin(compton->GetMax()));
      h_tmp->Add( prtmp, coef);   
      Int_t i = 0;
      for(auto err : errors){
	Double_t cnt = prtmp->GetBinContent(i);
	Double_t prEr = sqrt(cnt);
	err = sqrt(err*err + coef*coef*prEr*prEr);
        ++i;
      }      
    }
    for(unsigned int i=0; i<errors.size(); ++i){
      h_tmp->SetBinError(i,errors[i]);
    }
  }
  fHistArray->Add(h_tmp);
  fHist = h_tmp;
  fHist->SetXTitle("Time (ns)");
  fHist->SetYTitle("Count");
  fPeakRangeArray->Clear();
  fComptonRangeArray->Clear();
  draw();
  return fHist;
}

// New fit for a gamma-ray energy peak.
// If fixp is 0, it will fit by a function Gaussian + line.
// Otherwise, skewedness and a step background will be included.
void IsoAna::nfE(Int_t fixp){
  if(!fHist){
    std::cout << "No TH1 has been created." << std::endl;
    return;
  }
  ostringstream oss("");
  oss << "fit" << fFitArray->GetLast()+1;
  std::cout << oss.str() << "has been created." << std::endl;
  Double_t rangemin = fETmat->GetXaxis()->GetXmin();
  Double_t rangemax = fETmat->GetXaxis()->GetXmax();
  fFit = new TF1(oss.str().c_str(),"pol2(0) + [6]*gaus(3)/([5]*sqrt(2*TMath::Pi())) + (1-[6])*[3]*exp((x-[4])/[7])*TMath::Erfc((x-[4])/(sqrt(2)*[5]) + [5]/(sqrt(2)*[7])) + [3]*[8]*TMath::Erfc((x-[4])/(sqrt(2)*[5]))",rangemin, rangemax);
  fFitArray->Add(fFit);
  fFit->SetParNames("bg0", "bg1", "bg2", "area", "center", "sigma", "R", "beta", "step");
  fFit->SetParameters( 10, -0.01, 0,
    ((TMarker*)fMarkerArray->At(kNMarkers-3))->GetY(),
    ((TMarker*)fMarkerArray->At(kNMarkers-3))->GetX(), 1.0, 0, 5, 0);
  fFit->SetParLimits( 5, 0, 1000);
  fFit->SetLineWidth(1);
  fFit->SetLineColor(2);
  fFit->SetNpx(1000);
  gStyle->SetOptFit();
  if(fixp){
    fFit->FixParameter(2,0);
    fFit->FixParameter(6,1);
    fFit->FixParameter(7,1);
    fFit->FixParameter(8,0);
  }
  fHist->Fit(fFit,"","",
    ((TMarker*)fMarkerArray->At(kNMarkers-2))->GetX(),
    ((TMarker*)fMarkerArray->At(kNMarkers-1))->GetX());
  return;
}

// New fit with a function decay + constant
void IsoAna::nfT(Int_t fixp, Option_t *op, Double_t p1){
  if(!fHist){
    std::cout << "No TH1 has been created." << std::endl;
    return;
  }
  ostringstream oss("");
  oss << "fit" << fFitArray->GetLast()+1;
  std::cout << oss.str() << "has been created." << std::endl;
  Double_t rangemin = fETmat->GetXaxis()->GetXmin();
  Double_t rangemax = fETmat->GetXaxis()->GetXmax();
  fFit = new TF1(oss.str().c_str(),"[0]*pow(0.5, x/[1]) + [2]",rangemin,rangemax);
  fFitArray->Add(fFit);
  fFit->SetParNames("I0", "T1/2", "base line");
  fFit->SetParameters( 100, p1, 0 );
  fFit->SetLineWidth(1);
  fFit->SetLineColor(2);
  fFit->SetNpx(1000);
  gStyle->SetOptFit(1);
  if(fixp){
    fFit->FixParameter(2,0);
  }
  fHist->Fit(fFit,op,"",
    ((TMarker*)fMarkerArray->At(kNMarkers-2))->GetX(),
    ((TMarker*)fMarkerArray->At(kNMarkers-1))->GetX());
  if(!fixp){
    fFitBaseLine->SetParameter(0,fFit->GetParameter(2));
    fFitBaseLine->Draw("same");
  }
  return;
}

