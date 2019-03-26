#include "GAROOT/src/GPeakFitBase.h"

GPeakFitBase::GPeakFitBase(TH1F* hist) : TH1F(*(TH1F*)hist->Clone())
{
  fTF1Array = new TObjArray();
  fTF1Array->SetOwner();
}

GPeakFitBase::~GPeakFitBase(void){}


TF1* GPeakFitBase::ConstTF1(string f_name, Int_t pkn, Double_t xmin, Double_t xmax)
{
  if(xmax<0){
    xmin = GetXaxis()->GetBinCenter(0);
    xmax = GetXaxis()->GetBinCenter(GetNbinsX());
  }
  std::cout << "[GPeakFitBase::ConstTF1()]: Constructing new TF1 named " << f_name << std::endl;
  TF1* func;
  //construction of TF1
  func = new TF1(f_name.c_str(), "pol1(0)+gaus(2)", xmin, xmax);

  //setting parameter names
  func->SetParNames("bg0","bg1","area","center","sigma");
  for(int i=0; i<5; i++)
    func->SetParameter(i,0.0);
  func->SetParameter(0,-999);
  func->SetChisquare(0.0);

  return func;
}

void GPeakFitBase::SetTF1(TF1* func, Int_t fitn){
  fitn = SetFN(fitn);
  fTF1Array->RemoveAt(fitn);
  fTF1Array->AddAt(func, fitn);
  fTF1Curr = func;
  cout << "SetTF1(): " << fTF1Curr << " par0: " << fTF1Curr->GetParameter(0) << endl;
}

Int_t GPeakFitBase::SetFN(Int_t fitn)
{
  if(fitn < -1){
    fitn = 0;
  }
  if(fitn == -1){
    fitn = fTF1Array->IndexOf(fTF1Curr);
    if(fitn == -1)
      fitn = 0;
  }
  if(fitn > kNFitMax){
    std::cout << "[GPeakFitBase::SetFN()]: fit index" << fitn << " is too large.";
    std::cout << " The maximum number of fits is " << kNFitMax << "." << std::endl;
    return -1;
  }
  else {
    for(int i = fTF1Array->GetEntries(); i < fitn + 1; ++i){
      std::ostringstream oss("");
      oss << "fit_" << TH1F::GetName() << "_" << i;
      fTF1Array->Add( GPeakFitBase::ConstTF1(oss.str().c_str()) );
    }
    fTF1Curr = (TF1*)fTF1Array->At(fitn);
  }
  return fitn;
}

void GPeakFitBase::SetParameters(Double_t *par, Int_t fitn)
{
  SetFN(fitn);
  fTF1Curr->SetParameters( par );
}

Double_t GPeakFitBase::GFit(Option_t* option, Option_t* goption, Double_t xmin, Double_t xmax, Int_t fitn)
{
  fitn = SetFN(fitn);
  std::cout << "SetParameters() fn, ptr: " << fTF1Curr <<", " << fitn << std::endl;
  if(!xmin && !xmax)
    fTF1Curr->GetRange(xmin, xmax);
  const Int_t n_par = fTF1Curr->GetNpar();
  //debug
  for (int i=0; i < n_par; i++){
      std::cout<< "base param: " << fTF1Curr->GetParameter(i) << " " << i << std::endl;
  }
  Fit(fTF1Curr, option, goption, xmin, xmax);
  Double_t chisq = fTF1Curr->GetChisquare();
  std::cout << "[GPeakFitBase::GFit()]: Fitted by fTF1[" << fitn << "]." << std::endl;

  SetFN(fitn+1);
  return chisq;
}
