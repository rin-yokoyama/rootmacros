// A macro to add TMarker objects by clicking on a given histogram.
// Usage:
// >.L AddMarkersByClicking.C
// >AddMarkersByClicking(TH2F* hist, "text file name");
// Then Shift+Click to add a marker.
// The second argument is optional. Give the name of a text file with
// a list of x y positions of markers if you already have some.
// >mk->WriteTextFile("file name");
// to save x y positions of markers
// TCanvas event id can be different between systems.
// Please modify
const int kCLICK_ID = 7; //Shift + Click
// depending on your system

#include <iostream>
#include <fstream>
#include <string>

#include <TObjArray.h>
#include <TMarker.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH2.h>

class Markers {
public:
  Markers(){ array = nullptr; }
  ~Markers(){ if(array) delete array; }

  void Slot(Int_t event, Int_t x, Int_t y, TObject* selected);
  void LoadTextFile(string fname);
  void Draw();
  void RemoveLast();
  void WriteTextFile(string fname);

protected:
  TObjArray* array;
};

void Markers::LoadTextFile(string fname){
  if(array)
    delete array;
  array = new TObjArray();

  if(fname == "none")
    return;

  ifstream ifile(fname);
  if(!ifile){
    cout << "cannot open file: " << fname;
    return;
  }

  while(!ifile.eof()){
    Double_t x = 0;
    Double_t y = 0;
    ifile >> x;
    ifile >> y;
    array->Add(new TMarker(x,y,2));
  }
  ifile.close();

  return;
}

void Markers::Draw() {
  TIter next(array);
  while( TMarker* mk = (TMarker*)next() ){
    if(mk)
      mk->Draw("same");
  }
  return;
}

void Markers::Slot(Int_t event, Int_t x, Int_t y, TObject* selected) {
  // std::cout << "event: " << event << std::endl;
  if(event == kCLICK_ID){
    TCanvas *c = (TCanvas *) gTQSender;
    TPad *pad = (TPad *) c->GetSelectedPad();
    Float_t px = pad->AbsPixeltoX(x);
    Float_t py = pad->AbsPixeltoY(y);
    px = pad->PadtoX(px);
    py = pad->PadtoY(py);
    std::cout << "x,y: " << px << ", " << py << std::endl;
    TMarker* m = new TMarker(px,py,2);
    m->SetMarkerColor(kRed);
    array->Add(m);
    Draw();
  }
  return;
}

void Markers::WriteTextFile(string fname) {
  ofstream ofile(fname);
  TIter next(array);
  while( TMarker* mk = (TMarker*) next() ){
    if(mk)
      ofile << mk->GetX() << " " << mk->GetY() << endl;
  }
  ofile.close();
}

void Markers::RemoveLast(){
  delete array->At(array->GetLast());
  array->RemoveAt(array->GetLast());
  Draw();
  return;
}

Markers* mk;
TCanvas *canv;

void AddMarkersByClicking(TH2F* hist, std::string txt_file_name="none"){

  canv = new TCanvas("canv","canv");
  canv->SetLogz();
  hist->Draw("colz");
  gStyle->SetOptStat(0);
  mk = new Markers();
  mk->LoadTextFile(txt_file_name);
  mk->Draw();
  canv->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","Markers",mk,"Slot(Int_t,Int_t,Int_t,TObject*)");

  return;
}

