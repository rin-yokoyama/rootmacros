#include "GAROOT/src/Gf3ROOT.h"
#include "GAROOT/src/GPeakSearch.h"

Gf3ROOT::Gf3ROOT(TObjArray *hist_array)
{
  fGPeakFitArray = new TObjArray();
  fGPeakFitArray->SetOwner();
  TIter hist_next( hist_array );
  while ( TH1F* hist = (TH1F*)hist_next() ){
    GPeakFit* peak_fit = new GPeakFit( hist );
    //*peak_fit = gGHistLine;
    //*peak_fit = gGHistFill;
    fGPeakFitArray->Add( peak_fit );
  }
  fMkMouse = new MkMouse();
  fTCanvas = new TCanvas("Gcanv","GfROOT",CANV_W,CANV_H);
  gStyle->SetOptFit();
  fGPeakFitCurr = (GPeakFit*)fGPeakFitArray->First();
  ht(0);
  ConnectShortCut();
  mode = 0;
}

Gf3ROOT::~Gf3ROOT(void){}

void Gf3ROOT::ConnectShortCut(void)
{
  fTCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "Gf3ROOT", this, "ExecuteShortCut(Int_t, Int_t, Int_t, TObject*)");
}

void Gf3ROOT::Fit(Int_t event,Int_t px,Int_t py,TObject* sel)
{
  if(event == kButton3Down){
    Int_t mknum = fMkMouse->GetArray()->GetEntries();
    if(mknum<=2){
      std::cout << "[Gf3ROOT-E]: You need at reast 3 markers to fit." << std::endl;
      return;
    }
    fTCanvas->Disconnect();
    ConnectShortCut();
    fGPeakFitCurr->SetFitting(mknum-2, fMkMouse->GetArray());
    fGPeakFitCurr->GFit(op,gop);
    fGPeakFitCurr->Draw("");
    fTCanvas->Update();
  }
}

void Gf3ROOT::QFit(Int_t event,Int_t px,Int_t py,TObject* sel)
{
  TCanvas *c = (TCanvas *) gTQSender;
  TPad *pad = (TPad *) c->GetSelectedPad();
  if(event == kButton1Down){
    Float_t x = pad->AbsPixeltoX(px);
    Float_t y = pad->AbsPixeltoY(py);
    x = pad->PadtoX(x);
    y = pad->PadtoY(y);
    fGPeakFitCurr->SetFittingP1(x, y);
    fGPeakFitCurr->GFit("", "");
    fGPeakFitCurr->Draw("");
    fTCanvas->Update();
  }else if(event == kButton3Down){
    fGPeakFitCurr->SetFN( fGPeakFitCurr->GetFN() - 1 );
    std::cout << "Redo fTF1[" << fGPeakFitCurr->GetFN() << "]" << std::endl;
  }
}

void Gf3ROOT::Expand(Double_t x)
{
  if(mode!=2){
    xmin_com = x;
    mode = 2;
    return;
  }
  else if(mode == 2){ xmax_com = x;
    mode = 0;
    zoom(xmin_com,xmax_com);
    return;
  }
}

void Gf3ROOT::fn(Int_t fitn){ fGPeakFitCurr->SetFN(fitn); }

void Gf3ROOT::nf(Option_t* option, Option_t* goption)
{
  fMkMouse->Clear();
  fTCanvas->Disconnect();
  fTCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "MkMouse", fMkMouse, "SetMarker(Int_t,Int_t,Int_t,TObject*)");
  fTCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "Gf3ROOT", this, "Fit(Int_t, Int_t, Int_t, TObject*)");
  op = option;
  gop = goption;
  std::cout << "Set Markers for Fitting." << std::endl;
}

void Gf3ROOT::qf()
{
  fTCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", "Gf3ROOT", this, "QFit(Int_t, Int_t, Int_t, TObject*)");
  mode = 1;
  std::cout << "\n***Quick Fit Mode***\nPress ESC to end this mode." << std::endl;
}

void Gf3ROOT::ps(Double_t threshold, Int_t niter, Double_t sigma)
{
  TIter peak_fit_next( fGPeakFitArray );
  while( GPeakFit* peak_fit = (GPeakFit*) peak_fit_next() ){
    peak_fit->fGPeakSearch->SetParam(threshold, niter, sigma);
    peak_fit->PeakSearch();
  }
}

void Gf3ROOT::mf(Option_t* opt,Option_t* gopt)
{
  TIter peak_fit_next( fGPeakFitArray );
  while( GPeakFit* peak_fit = (GPeakFit*) peak_fit_next() ){
    peak_fit->MkFit(opt,gopt);
    fTCanvas->Update();
  }
}

void Gf3ROOT::esc()
{
  std::cout << "\nESCAPE" << std::endl;
  fTCanvas->Disconnect();
  ConnectShortCut();
  mode = 0;
}

void Gf3ROOT::ht(Int_t id)
{
  if(id != -1){
    GPeakFit* peak_fit = (GPeakFit*)fGPeakFitArray->At(id);
    if(!peak_fit){
      std::cout << "[Gf3ROOT::ht()]: hist id " << id << " is out of bounds." << std::endl;
      return;
    }
      fGPeakFitCurr = peak_fit;
  }
  fGPeakFitCurr->Draw("");
  fTCanvas->Update();
}

void Gf3ROOT::hn()
{
  ht(fGPeakFitArray->IndexOf(fGPeakFitCurr)+1);
}

void Gf3ROOT::hb()
{
  ht(fGPeakFitArray->IndexOf(fGPeakFitCurr)-1);
}

void Gf3ROOT::fn()
{
    fGPeakFitCurr->SetFN( fGPeakFitCurr->GetFN() + 1 );
    std::cout << "Next Fit: fTF1[" << fGPeakFitCurr->GetFN() << "]" << std::endl;
}

void Gf3ROOT::fb()
{
    fGPeakFitCurr->SetFN( fGPeakFitCurr->GetFN() - 1 );
    std::cout << "Next Fit: fTF1[" << fGPeakFitCurr->GetFN() << "]" << std::endl;
}

void Gf3ROOT::zoom(Double_t low, Double_t up)
{
  TIter peak_fit_next( fGPeakFitArray );
  while( GPeakFit* peak_fit = (GPeakFit*)peak_fit_next() ){
    peak_fit->SetAxisRange(low,up);
  }
  ht();
  std::cout << " zoomed " << std::endl;
}

void Gf3ROOT::unzoom()
{
  TIter peak_fit_next( fGPeakFitArray );
  while( GPeakFit* peak_fit = (GPeakFit*)peak_fit_next() ){
    peak_fit->GetXaxis()->UnZoom();
    peak_fit->GetYaxis()->UnZoom();
  }
  ht();
}

void Gf3ROOT::WriteCSV(string ofname, Int_t fidlow, Int_t fidup) const
{
  if(fidup<0)
    fidup = fGPeakFitCurr->GetTF1Array()->GetEntries()-1;
  std::ofstream ofile(ofname.c_str(), ios::out);
  //finding a fTF1 with the largest number of peaks.
  Int_t fmax[3] = {0,0,0};
  {
    int i=0;
    TIter peak_fit_next( fGPeakFitArray );
    while( GPeakFit* peak_fit = (GPeakFit*)peak_fit_next() ){
      for(int j=fidlow; j<fidup+1; j++){
        if(fmax[2] < peak_fit->GetTF1(j)->GetNpar()){
          fmax[0] = i;
          fmax[1] = j;
          fmax[2] = peak_fit->GetTF1(j)->GetNpar();
        }
      }
      i++;
    }
  }

  //writes output csv file.
  ofile << "Hist_N, Fit_N, Chisquare, ";
  for(int i=0; i<fmax[2]; i++){
    ofile << ((GPeakFit*)fGPeakFitArray->At(fmax[0]))->GetTF1(fmax[1])->GetParName(i) << ", ";
    ofile << "error, ";
  }
  ofile << std::endl;
  {
    int i=0;
    TIter peak_fit_next( fGPeakFitArray );
    while( GPeakFit* peak_fit = (GPeakFit*)peak_fit_next() ){
      for(int j=fidlow; j<fidup+1; j++){
        ofile << i << ", " << j << ", ";
        ofile << peak_fit->GetTF1(j)->GetChisquare() << ", ";
        for(int k=0; k<fmax[2]; k++){
          ofile << peak_fit->GetTF1(j)->GetParameter(k) << ", ";
          ofile << peak_fit->GetTF1(j)->GetParError(k) << ", ";
        }
        ofile << std::endl;
      }
      i++;
    }
  }
  ofile.close();

}

//void Gf3ROOT::RoughCalib(string ofname)
//{
//  Int_t f[2], s[2];
//  GSource *fGSource[2];
//  std::cout << "chose two peaks for rough calibration." << std::endl;
//  std::cout << "input the first fTF1 number:";
//  cin >> f[0];
//  GSourceList* fSList = new GSourceList(AppendMacroPath(SRC_LIST));
//  if(!fSList->GetNSource()){ return; }
//  fSList->List();
//  std::cout << "chose one of the sources from the above." << std::endl;
//  std::cout << "input source#: ";
//  cin >> s[0];
//  fGSource[0] = new GSource(fSList->GetSource(s[0]));
//  fGSource[0]->ListEnergy();
//
//  std::cout << "chose one of the gamma from the above." << std::endl;
//  std::cout << "input #: ";
//  cin >> s[0];  
//  std::cout << "input the second fTF1 number:";
//  cin >> f[1];
//  GSourceList* fSList = new GSourceList(AppendMacroPath(SRC_LIST));
//  fSList->List();
//  std::cout << "chose one of the sources from the above." << std::endl;
//  std::cout << "input source#: ";
//  cin >> s[1];
//  fGSource[1] = new GSource(fSList->GetSource(s[1]));
//  fGSource[1]->ListEnergy();
//  std::cout << "chose one of the gamma from the above." << std::endl;
//  std::cout << "input #: ";
//  cin >> s[1];
//  GRoughCalib* fRCalib = new GRoughCalib(fGSource[0], s[0], fGSource[1], s[1]);
//  fRCalib->RoughCalib2P(this, f[0], f[1]);
//  fRCalib->WritePrmFile(ofname);
//  return;
//}

void Gf3ROOT::PrintAll(string ps_file)
{
  string ps_file_beg = ps_file + "[";
  fTCanvas->Print(ps_file_beg.c_str());
  TIter peak_fit_next( fGPeakFitArray );
  while( GPeakFit* peak_fit = (GPeakFit*)peak_fit_next() ){
    ht( fGPeakFitArray->IndexOf(peak_fit) );
    fTCanvas->Print(ps_file.c_str());
  }
  string ps_file_end = ps_file+ "]";
  fTCanvas->Print(ps_file_end.c_str());
}

void Gf3ROOT::ListMkPos(void) const
{
  TIter peak_fit_next( fGPeakFitArray );
  while( GPeakFit* peak_fit = (GPeakFit*)peak_fit_next() ){
    std::cout << ((TMarker*)peak_fit->GetArray()->At(0))->GetX() << " " << ((TMarker*)peak_fit->GetArray()->At(0))->GetY() << std::endl;
  }
  return;
}

void Gf3ROOT::SaveFits(Int_t fidlow, Int_t fidup, string ffname) const
{
  TFile *ffile = new TFile(ffname.c_str(),"recreate");
  if(fidup<0)
    fidup = fGPeakFitCurr->GetTF1Array()->GetEntries()-1;
  cout << "fidup: " << fidup << endl;
  for(int i=fidlow; i<fidup; i++){
    ostringstream oss;
    oss << "ffTF1_" << i - fidlow;
    fGPeakFitCurr->GetTF1(i)->Write(oss.str().c_str());
    cout << "par0: " << fGPeakFitCurr->GetTF1(i)->GetParameter(0) << endl;
  }
  ffile->Close();
  return;
}

void Gf3ROOT::FileFit(string ffname, string option)
{
  TIter peak_fit_next( fGPeakFitArray );
  while( GPeakFit* peak_fit = (GPeakFit*)peak_fit_next() ){
    ROOTFileReader *Rf = new ROOTFileReader(ffname);
    Int_t nfit = Rf->ReadObj("TF1","TF2");
    peak_fit->SetFN(0);
    for(int j=0; j<nfit; j++){
      peak_fit->SetTF1((TF1*)Rf->GetObj(j)->Clone(),j);
      peak_fit->GFit(option.c_str());
    }
    peak_fit->Draw();
  }
  return;
}
