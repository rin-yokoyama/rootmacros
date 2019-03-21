#ifndef GAROOT_GF3ROOT_H
#define GAROOT_GF3ROOT_H

#include <iostream>
#include <fstream>
#include "GAROOT/src/subfunctions.h"
#include "GAROOT/src/setgaroot.h"
#include "GAROOT/src/GPeakFit.h"
#include "GAROOT/src/MkMouse.h"
#include "GAROOT/src/GPeakSearch.h"
//#include "GAROOT/src/GSourceList.h"
//#include "GAROOT/src/GRoughCalib.h"
#include "GAROOT/src/ROOTFileReader.h"
#include <TCanvas.h>
#include <TObjArray.h>
#include <TMarker.h>
#include <TStyle.h>
#include <TPad.h>
#include <RQ_OBJECT.h>

class Gf3ROOT
{
  RQ_OBJECT("Gf3ROOT")
 protected:
  MkMouse *fMkMouse;
  TCanvas *fTCanvas;
  TObjArray *fGPeakFitArray;
  GPeakFit *fGPeakFitCurr;

  Int_t mode; //current mode. (0:normal, 1:qf mode)
  Double_t xmin_com, xmax_com; //common range
  Option_t* op; //option for fitting
  Option_t* gop; //goption for fitting
  void ConnectShortCut();

 public:

  Gf3ROOT(TObjArray *hist_array);
  virtual ~Gf3ROOT(void);
  void ExecuteShortCut(Int_t event,Int_t px,Int_t py,TObject* sel);
  Int_t GetNHist() const { return fGPeakFitArray->GetEntries(); }
  TObjArray * GetArray() const { return fGPeakFitArray; }
  void fn(Int_t fitn);
  void nf(Option_t* option = "", Option_t* goption = ""); //new fit
  void qf(); //sets quick fit mode #2
  void ps(Double_t threshold  = -1, Int_t niter = -1, Double_t sigma = -1); //peak search
  void mf(Option_t* opt = "V", Option_t* gopt=""); //fit markers
  void esc(); //escapes
  void ht(Int_t id = -1); //draws histogram
  void hn(); //next histogram
  void hb(); //previous histogram
  void fn(); //next TF1
  void fb(); //previous TF1
  void zoom(Double_t low, Double_t up); //zoom all histograms
  void unzoom(); //unzoom all histograms
  void Fit(Int_t event,Int_t px,Int_t py,TObject* sel); //Fit
  void WriteCSV(string ofname, Int_t fidlow = 0, Int_t fidup = NF_MAX) const; //writes fitting parameters into a csv file.
//  void RoughCalib(string ofname); //2 peak calibration
  void PrintAll(string ps_file); //prints all histograms as a ps file
  void ListMkPos(void) const; //lists x, y value of fTMarker[0]
  void SaveFits(Int_t fidlow = 0, Int_t fidup = -1, string ffname = "GfROOTfits.root") const; //saves TF1s in the fGPeakFit[hid].
  void FileFit(string ffname = "GfROOTfits.root",string option=""); //fits all histograms by TF1s in the file
  void QFit(Int_t event,Int_t px,Int_t py,TObject* sel); //QuickFit
  void Expand(Double_t x);
};

#endif
