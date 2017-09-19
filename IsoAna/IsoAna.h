#ifndef ISOANA_H
#define ISOANA_H

#include <iostream>
#include <sstream>
#include <vector>
#include <TStyle.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TMarker.h>
#include <TCanvas.h>
#include <TObjArray.h>
#include <TMath.h>

class Range : public TObject {
private:
  Double_t fMin;
  Double_t fMax;

public:
  Range(Double_t min, Double_t max){ Set(min,max); }
  ~Range(){}
  void Set(Double_t min, Double_t max){ fMin=min; fMax=max; }
  Double_t GetMin() const {return fMin;}
  Double_t GetMax() const {return fMax;}
  Double_t GetWidth() const {return fMax - fMin;}
};

class IsoAna{
private:
  TH2F *fETmat;
  TObjArray *fHistArray;
  TH1D* fHist;

  TObjArray *fFitArray;
  TF1* fFit;
  TF1* fFitBaseLine;

  TObjArray *fPeakRangeArray;
  TObjArray *fComptonRangeArray;

  Color_t hl_col, hf_col;
  TObjArray *fMarkerArray;
  Int_t fNMarkerSet;

  static const int kNMarkers = 3;

  Int_t fNextFixp;
  Option_t *fNextOp;
  Double_t fNextP1;

public:
  TCanvas *canv;
  IsoAna( TH2F* ETin );
  ~IsoAna(void);
  void dmk(void);
  void mkc(void) const;
  void draw() const;
  void sethcol(Color_t lcol, Color_t fcol);
  void setpk(Double_t pklow = 0, Double_t pkup = 0);
  void setct(Double_t pklow = 0, Double_t pkup = 0);
  TH1D* prE(Double_t tlow = 0, Double_t tup = 0);
  TH1D* prT();
  void nfE(Int_t fixp = 1, Option_t *op="");
  void nfT(Int_t fixp = 1, Option_t *op = "", Double_t p1 = 500);
  void SetMarker(Int_t event,Int_t px,Int_t py,TObject* sel);

protected:
  void Init(void);

};

#endif
