#include "GAROOT/src/GPeakFit.h"

GPeakFit::GPeakFit(TH1F* hist) : GPeakFitBase(hist)
{
  fERes = E_RES;
  for(int i=0; i<6*NP_MAX+3; i++){
    fFixPar[i] = 0;
    fFixParVal[i] = 0.0;
  }
  fMarkerArray = new TObjArray();
  fGPeakSearch = new GPeakSearch();
}

GPeakFit::~GPeakFit(void){}

TF1* GPeakFit::ConstTF1(string f_name, Int_t npk, Double_t xmin, Double_t xmax)
{
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
    int j = 6*i;
    f_expr << "+[" << j+6 << "]*[" << 6*npk+3 << "]*gaus(" << j+3 << ")/([" << j+5 << "]*sqrt(2*TMath::Pi()))+(1-[" << j+6 << "])*[" << j+3 << "]*[" << 6*npk+3 << "]*exp((x-[" << j+4 << "])/[" << j+7 << "])*TMath::Erfc((x-[" << j+4 << "])/(sqrt(2)*[" << j+5 << "])+[" << j+5 << "]/(sqrt(2)*[" << j+7 << "]))+[" << j+3 << "]*[" << 6*npk+3 << "]*[" << j+8 << "]*TMath::Erfc((x-[" << j+4 << "])/(sqrt(2)*[" << j+5 << "]))";
  }

  //construction of TF1
  func = new TF1(f_name.c_str(), f_expr.str().c_str(), xmin, xmax);
  //*func = gGFitLine;

  //setting parameter names
  for(int i=0; i<3; i++){
    f_parname.str("");
    f_parname << "BG" << i;
    func->SetParName(i, f_parname.str().c_str());
  }
  string parstr[6] = {"area","center","sigma","R","beta","step"};
  for(int i=0; i<npk; i++){
    for(int j=0; j<6; j++){
      f_parname.str("");
      f_parname << parstr[j] << i;
      func->SetParName(6*i+j+3, f_parname.str().c_str());
    }
    //func->SetParLimits(6*i+3, 0, 10000000000);
    //func->SetParLimits(6*i+5, 0, 1000000);
  }
  func->SetParName(6*npk+3, "gain");

  //initialization of the parameters
  for(int i=0; i<npk*6+4; i++)
    func->SetParameter(i,1.0);
 
  const int idc = fTF1Array->IndexOf(fTF1Curr);
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
  Int_t npk = (fTF1Curr->GetNpar() - 3)/6;
  ifstream fin(AppendMacroPath(pf_name).c_str(), ios::in);
  cout << "[GPeakFit::FixParams()]: open fixpar file: " << AppendMacroPath(pf_name).c_str() << endl;
  if(!fin){ cout << "[GPeakFit::FixParams()]: cannot open fixpar file." << endl; return; }
  for(int i=0; i<6*npk+3; i++){
    fin >> fFixPar[i];
    fin >> fFixParVal[i];
    if(fin.eof()){
      cout << "[GPeakFit::FixParams()]: invalid fixpar file." << endl;
      fin.close();
      return;
    }
  }
  fin.close();
  for(int i=0; i<6*npk+3; i++){
    if(fFixPar[i])
      fTF1Curr->FixParameter(i, fFixParVal[i]);
  }
  fTF1Curr->FixParameter(6*npk+3, GetBinWidth(0));
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
  fTF1Curr->SetParameter(0,(mkpos[1][0]*mkpos[0][1] - mkpos[1][1]*mkpos[0][0])/(mkpos[0][1]-mkpos[0][0]));
  fTF1Curr->SetParameter(1, (mkpos[1][1]-mkpos[1][0])/(mkpos[0][1]-mkpos[0][0]));
  fTF1Curr->SetParameter(2, 0.0);
  for(int i=0; i<mk_array->GetEntries()-2; i++){
    Double_t mkposi[2] = {((TMarker*)mk_array->At(i+2))->GetX(),((TMarker*)mk_array->At(i+2))->GetY()}; 
    fTF1Curr->SetParameter(6*i+3, mkposi[1] * 
       	(
       	sqrt(2*3.1415926535)*( mkposi[0] * fERes + E_RES_MIN)
       	/GetBinWidth(0)
       	)
       	);
    fTF1Curr->SetParLimits(6*i+3, 0, 1E+10); 
    fTF1Curr->SetParameter(6*i+4, mkposi[0]);
    fTF1Curr->SetParLimits(6*i+4, mkpos[0][0], mkpos[0][1]);
    fTF1Curr->SetParameter(6*i+5, mkposi[0]*fERes+E_RES_MIN);
    fTF1Curr->SetParLimits(6*i+5, 0, mkposi[0]);
    fTF1Curr->SetParameter(6*i+6, 0.9);
    fTF1Curr->SetParameter(6*i+7, 0.5);
    fTF1Curr->SetParameter(6*i+8, 0.01);
  }
  FixParams();
}

void GPeakFit::Draw(Option_t* option)
{
  TH1::Draw(option);
  TIter fit_next( fTF1Array );
  while( TF1* fit = (TF1*)fit_next() ){
    if(fit->GetChisquare()!=0){
      fit->Draw("same");
      // Drawing BG curves.
      Int_t npk = (fTF1Curr->GetNpar() - 3)/6;
      ostringstream bg_expr;
      bg_expr.str("");
      bg_expr << "pol2(0)";
      for(int i=0; i<npk; i++){
        int j = 4*i;
        bg_expr << "+[" << j+3 << "]*[" << 4*npk+3 << "]*[" << j+6 << "]*TMath::Erfc((x-[" << j+4 << "])/(sqrt(2)*[" << j+5 << "]))";
      }
      string bgname = GetName();
      bgname.append("_bg");
      fTF1BG = new TF1(bgname.c_str(), bg_expr.str().c_str(), fTF1Curr->GetXmin(), fTF1Curr->GetXmax());
      //*fTF1BG = gGBGLine;
    
      fTF1BG->SetParameter(0, fTF1Curr->GetParameter(0));
      fTF1BG->SetParameter(1, fTF1Curr->GetParameter(1));
      fTF1BG->SetParameter(2, fTF1Curr->GetParameter(2));
      for(int i=0; i<npk; i++){
        int j=6*i;
        int k=4*i;
        fTF1BG->SetParameter(k+3, fTF1Curr->GetParameter(j+3));
        fTF1BG->SetParameter(k+4, fTF1Curr->GetParameter(j+4));
        fTF1BG->SetParameter(k+5, fTF1Curr->GetParameter(j+5));
        fTF1BG->SetParameter(k+6, fTF1Curr->GetParameter(j+8));
      }
      fTF1BG->SetParameter(4*npk+3, fTF1Curr->GetParameter(6*npk+3));
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
