#include "GAROOT/src/GPeakFit.h"

GPeakFit::GPeakFit(TH1F* hist) : GPeakFitBase(hist)
{
  fERes = E_RES;
  for(int i=0; i<kNPAR*NP_MAX+3; i++){
    fFixPar[i] = 0;
    fFixParVal[i] = 0.0;
  }
  fMarkerArray = new TObjArray();
  fGPeakSearch = new GPeakSearch();
}

GPeakFit::~GPeakFit(void){}

TF1* GPeakFit::ConstTF1(string f_name, Int_t npk, Double_t xmin, Double_t xmax)
{
  if(xmax<0){
    xmin = GetXaxis()->GetBinCenter(0);
    xmax = GetXaxis()->GetBinCenter(GetNbinsX());
  }
  TF1* func;
  ostringstream f_expr, f_parname;
  // creating function expression
  if(npk > NP_MAX){
    cout << "[GPeakFit-E]: invalid number of peaks." << endl;
    npk = 1;
  }
  f_expr.str("");
  f_expr << "pol2(0)";
  for(int i=0; i<npk; i++){
    int j = kNPAR*i;
    f_expr << "+[" << kNPAR*npk+3 << "]*gaus(" << j+3 << ")/([" << j+5 << "]*sqrt(2*TMath::Pi()))";
  }

  //construction of TF1
  func = new TF1(f_name.c_str(), f_expr.str().c_str(), xmin, xmax);
  func->SetNpx(1000);
  //*func = gGFitLine;

  //setting parameter names
  for(int i=0; i<kNPAR; i++){
    f_parname.str("");
    f_parname << "BG" << i;
    func->SetParName(i, f_parname.str().c_str());
  }
  string parstr[kNPAR] = {"area","center","sigma"};
  for(int i=0; i<npk; i++){
    for(int j=0; j<kNPAR; j++){
      f_parname.str("");
      f_parname << parstr[j] << i;
      func->SetParName(kNPAR*i+j+3, f_parname.str().c_str());
    }
  }
  func->SetParName(kNPAR*npk+3, "gain");

  //initialization of the parameters
  for(int i=0; i<npk*kNPAR+4; i++)
    func->SetParameter(i,1.0);
 
  const int idc = fTF1Array->IndexOf(fTF1Curr);
  cout << "[GPeakFit::ConstTF1()]: constructed new function " << f_name << " at fTF1[" << idc << "]" << endl;
  fTF1Array->RemoveAt(idc);
  fTF1Array->AddAt(func,idc);

  return (TF1*)func;
}

void GPeakFit::ClearTF1(Int_t fitn)
{
  if(SetFN(fitn)){ return; }
  string fname(fTF1Curr->GetName());
  fTF1Curr = GPeakFit::ConstTF1(fname);
}

void GPeakFit::FixParams(string pf_name)
{
  Int_t npk = (fTF1Curr->GetNpar() - 3)/kNPAR;
  ifstream fin(AppendMacroPath(pf_name).c_str(), ios::in);
  cout << "[GPeakFit::FixParams()]: open fixpar file: " << AppendMacroPath(pf_name).c_str() << endl;
  if(!fin){ cout << "[GPeakFit::FixParams()]: cannot open fixpar file." << endl; return; }
  for(int i=0; i<kNPAR*npk+3; i++){
    fin >> fFixPar[i];
    fin >> fFixParVal[i];
    if(fin.eof()){
      cout << "[GPeakFit::FixParams()]: invalid fixpar file." << endl;
      fin.close();
      return;
    }
  }
  fin.close();
  for(int i=0; i<kNPAR*npk+3; i++){
    if(fFixPar[i])
      fTF1Curr->FixParameter(i, fFixParVal[i]);
  }
  fTF1Curr->FixParameter(kNPAR*npk+3, GetBinWidth(0));
}

void GPeakFit::SetFitting(Int_t npk, const TObjArray *mk_array, Int_t fitn )
{
  if(SetFN(fitn)<0){ return; }
  string fname = fTF1Curr->GetName();
  Double_t mkpos[2][2];
  mkpos[0][0] = ((TMarker*)mk_array->At(0))->GetX();
  mkpos[1][0] = ((TMarker*)mk_array->At(0))->GetY();
  mkpos[0][1] = ((TMarker*)mk_array->At(1))->GetX();
  mkpos[1][1] = ((TMarker*)mk_array->At(1))->GetY();
  fTF1Curr = GPeakFit::ConstTF1(fname, npk, mkpos[0][0], mkpos[0][1]);
  const Double_t p0 = (mkpos[1][0]*mkpos[0][1] - mkpos[1][1]*mkpos[0][0])/(mkpos[0][1]-mkpos[0][0]);
  const Double_t p1 = (mkpos[1][1]-mkpos[1][0])/(mkpos[0][1]-mkpos[0][0]);
  fTF1Curr->SetParameter(0, p0);
  fTF1Curr->SetParameter(1, p1);
  fTF1Curr->SetParameter(2, 0.0);

  const Int_t bin_first = GetXaxis()->FindBin(mkpos[0][0]);
  const Int_t bin_last = GetXaxis()->FindBin(mkpos[0][1]);
  Double_t area = Integral(bin_first,bin_last)*GetBinWidth(0);
  area = area + (p0 + p1*mkpos[0][0])*mkpos[0][0] - (p0 + p1*mkpos[0][1])*mkpos[0][1];
  const Double_t avg_count = area/GetBinWidth(0);
  Double_t fwhm = 0;
  for(int i=bin_first; i<bin_last; i++){
    if(GetBinContent(i)>avg_count+p0+p1*GetBinCenter(i))
       fwhm++;
  }
  fwhm = fwhm * GetBinWidth(0);
  Double_t peak_height_sum = 0;
  for(int i=0; i<mk_array->GetEntries()-2; i++){
    peak_height_sum += ((TMarker*)mk_array->At(i+2))->GetY();
  }

  for(int i=0; i<mk_array->GetEntries()-2; i++){
    Double_t mkposi[2] = {((TMarker*)mk_array->At(i+2))->GetX(),((TMarker*)mk_array->At(i+2))->GetY()}; 
    //fTF1Curr->SetParameter(kNPAR*i+3, mkposi[1] * 
    //   	(
    //   	sqrt(2*3.1415926535)*( mkposi[0] * fERes + E_RES_MIN)
    //   	/GetBinWidth(0)
    //   	)
    //   	);
    fTF1Curr->SetParameter(kNPAR*i+3, mkposi[1]/peak_height_sum*area);
    fTF1Curr->SetParLimits(kNPAR*i+3, 0, 1E+3*fTF1Curr->GetParameter(6*i+3)); 
    fTF1Curr->SetParameter(kNPAR*i+4, mkposi[0]);
    fTF1Curr->SetParLimits(kNPAR*i+4, mkpos[0][0], mkpos[0][1]);
    fTF1Curr->SetParameter(kNPAR*i+5, mkposi[0]*fERes+E_RES_MIN);
    //fTF1Curr->SetParameter(kNPAR*i+5, fwhm/2.35);
    fTF1Curr->SetParLimits(kNPAR*i+5, 0, mkpos[0][1]-mkpos[0][0]);
  }
  FixParams();
}

void GPeakFit::Draw(Option_t* option)
{
  TH1::Draw(option);
  TIter fit_next( fTF1Array );
  while( TF1* fit = (TF1*)fit_next() ){
    if(fit->GetChisquare()>0){
      fit->Draw("same");
      // Drawing BG curves.
      Int_t npk = (fTF1Curr->GetNpar() - 3)/kNPAR;
      ostringstream bg_expr;
      bg_expr.str("");
      bg_expr << "pol2(0)";
      string bgname = GetName();
      bgname.append("_bg");
      fTF1BG = new TF1(bgname.c_str(), bg_expr.str().c_str(), fTF1Curr->GetXmin(), fTF1Curr->GetXmax());
      //*fTF1BG = gGBGLine;
    
      fTF1BG->SetParameter(0, fTF1Curr->GetParameter(0));
      fTF1BG->SetParameter(1, fTF1Curr->GetParameter(1));
      fTF1BG->SetParameter(2, fTF1Curr->GetParameter(2));
      fTF1BG->Draw("same");
    }
  }
  TIter mk_next( fMarkerArray );
  while( TMarker* marker = (TMarker*)mk_next()){
    if(marker->GetX() != 0)
      marker->Draw("same");
  }
  return;
}

void GPeakFit::PeakSearch(void)
{
  TIter mk_next( fMarkerArray );
  while( TMarker* marker = (TMarker*)mk_next()){
    marker->SetX(0);
  }
  fGPeakSearch->PSearch((TH1*)this);
  fGPeakSearch->GetPMarker(fMarkerArray);
  Draw("");
  fGPeakSearch->GetBG()->Draw("same");
}

void GPeakFit::SetFittingP1(Double_t x, Double_t y)
{
  TObjArray* mk_array = new TObjArray();
  TMarker *mk = new TMarker();
  mk->SetX(x - (x*(E_RES)+E_RES_MIN)*QF_R);
  mk->SetY( GetBinContent( FindBin( mk->GetX() ) ));
  mk_array->Add(mk);
  mk = new TMarker();
  mk->SetX(x + (x*(E_RES)+E_RES_MIN)*QF_R);
  mk->SetY( GetBinContent( FindBin( mk->GetX() ) ));
  mk_array->Add(mk);
  mk = new TMarker();
  mk->SetX(x);
  mk->SetY(y);
  mk_array->Add(mk);
  SetFitting(1, mk_array);
  return;
}

void GPeakFit::MkFit(Option_t* op, Option_t* gop)
{
  TIter mk_next( fMarkerArray );
  while( TMarker* marker = (TMarker*)mk_next()){
    if( marker->GetX() != 0){
      SetFittingP1(marker->GetX(), marker->GetY());
      GFit(op,gop);
    }
  }
  Draw("");
  return;
}

Double_t GPeakFit::GetMkX(Int_t mkn) const
{
  return ((TMarker*)fMarkerArray->At(mkn))->GetX();
}
