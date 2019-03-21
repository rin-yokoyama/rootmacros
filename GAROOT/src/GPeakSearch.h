#ifndef GAROOT_GPEAKSEARCH_H
#define GAROOT_GPEAKSEARCH_H

#include<TSpectrum.h>
#include<TH1.h>
#include<TMarker.h>
#include<TObjArray.h>
#include"GAROOT/src/AOrder.h"
#include"GAROOT/src/Styles.h"

class GPeakSearch : public TSpectrum
{
 protected:
  Int_t niter;
  Double_t sigma;
  Double_t threshold;
  TH1F* fTH1;
  TH1F* fTH1BG;
 public:
  GPeakSearch(void);
  ~GPeakSearch(void);
  Int_t PSearch(TH1*);
  void SetParam(Double_t thresh = -1, Int_t nit = -1, Double_t sig = -1);
  TH1F* GetBG(void) const { return fTH1BG; }
  void GetPMarker(TObjArray* marker_array) const;
};

#endif
