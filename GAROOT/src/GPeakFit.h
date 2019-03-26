#ifndef GAROOT_GPEAKFIT_H
#define GAROOT_GPEAKFIT_H

#include <fstream>
#include <sstream>
#include <TROOT.h>
#include <TMarker.h>
#include "GAROOT/src/setgaroot.h"
#include "GAROOT/src/Styles.h"
#include "GAROOT/src/subfunctions.h"
#include "GAROOT/src/GPeakFitBase.h"
#include "GAROOT/src/GPeakSearch.h"

class GPeakFit : public GPeakFitBase
{
 protected:
  static const int kNPAR = 3;
  TF1* fTF1BG; //TF1 for drawing background.
  TObjArray* fMarkerArray;

  TF1* ConstTF1(string f_name, Int_t npk = 1, Double_t xmin = 0, Double_t xmax = -1); //over rides fitting function with gf3-like function
  Double_t fERes; //energy resolution in sigma (0.001 in default)
  Int_t fFixPar[kNPAR*NP_MAX+3];
  Double_t fFixParVal[kNPAR*NP_MAX+3];

 public:
  GPeakSearch* fGPeakSearch;
  GPeakFit(TH1F* hist);
  virtual ~GPeakFit(void);
  void ClearTF1(Int_t fitn = -1); //reconstructs fTF1[n].
  void FixParams(string pf_name = FIX_PRM); //fixes parameters according to the fixpar file.
  void SetFitting(Int_t npk, const TObjArray* mk_array, Int_t fitn = -1); //sets initial parameters.
  void Draw(Option_t* option = ""); //overrides Draw() function of TH1
  void PeakSearch(void); //peak search
  GPeakSearch* GetPeakSearch(void) const { return fGPeakSearch; }
  void SetFittingP1(Double_t x, Double_t y); //set parameters from one position
  void MkFit(Option_t* op,Option_t* gop); //peak fit for marker positions
  Double_t GetMkX(Int_t mkn) const; //returns x value of fTMarker
  TObjArray* GetArray() const { return fMarkerArray; }
  void SetERes(const Double_t &resolution){ fERes = resolution; }
  Double_t GetERes(void) const { return fERes; }
};

#endif
