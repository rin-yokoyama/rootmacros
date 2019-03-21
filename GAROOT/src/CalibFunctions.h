#ifndef GAROOT_CALIBFUNCTIONS_H
#define GAROOT_CALIBFUNCTIONS_H

#include<iostream>
#include<string>
#include<sstream>
#include<TF1.h>


enum CalFuncType {
  kLin = 1, kPol2 = 2, kPol3 = 3, kPol4 = 4, kPol5 = 5,
  k2Lin = 51, kHyperbolic = 52
};

Double_t TwoLine(Double_t *x, Double_t *par );

class GCalibFunc
{
 protected:
  Int_t f_num;
  string f_name;
  Double_t range[2];
 public:
  GCalibFunc(string fname = "GCalib", Int_t fnum = 0);
  ~GCalibFunc(void);
  void SetRange(Double_t xlow, Double_t xup);
  TF1* CalibFunc(CalFuncType func_type);
  Double_t CalcResidualError(Double_t ch, Double_t che, Double_t RefEe, Double_t* par, Double_t*parE, CalFuncType func_type);
  void SetInitalParam(TF1* fTF1, Double_t gain, Double_t offset, CalFuncType func_type);
};

#endif
