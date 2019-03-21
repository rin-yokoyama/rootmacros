#ifndef GAROOT_GPEAKFITBASE_H
#define GAROOT_GPEAKFITBASE_H

#include <iostream>
#include <string>
#include <sstream>
#include <TH1.h>
#include <TF1.h>
#include <TObjArray.h>

//This is a base class for peak fitting of a gamma spectrum.

class GPeakFitBase : public TH1F
{
 private:
  static const int kNFitMax = 100;

 protected:
  TObjArray *fTF1Array = nullptr; // an array of fit functions
  TF1* fTF1Curr = nullptr; // pointer to the current TF1

 public:
  GPeakFitBase(TH1F* hist);
  virtual ~GPeakFitBase(void);
  TF1* GetTF1(Int_t fitn = -1); //returns fTF1[fn]
  void SetTF1(TF1* func, Int_t fitn = -1);
  Int_t GetFN() const { return fTF1Array->IndexOf(fTF1Curr); }
  Int_t SetFN(Int_t fitn);
  void SetParameters(Double_t *par, Int_t fitn = -1); //sets par as parameters
  virtual Double_t GFit(Option_t* option = "", Option_t* goption = "", Double_t xmin = 0, Double_t xmax = 0, Int_t fitn = -1); //fits this TH1 with fTF1[fnum]
  virtual TF1* ConstTF1(string f_name,Int_t pkn = 1, Double_t xmin = 0, Double_t xmax = 10000);   // This will create a fitting function for gamma peaks.
  TObjArray* GetTF1Array() const { return fTF1Array; }
};

inline TF1* GPeakFitBase::GetTF1(Int_t fitn){
  SetFN(fitn);
  return (TF1*)fTF1Curr;
}

#endif
