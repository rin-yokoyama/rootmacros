#include"CalibFunctions.h"

Double_t TwoLine( Double_t *x, Double_t *par ){
  Float_t xx = x[0];
  Double_t f;
  if( xx < par[0] )
    f = par[2]*xx + par[1];
  else
    f = par[4]*xx + par[3];
  return f;
}

GCalibFunc::GCalibFunc(string fname, Int_t fnum)
{
  f_num = fnum;
  f_name = fname;
  range[0] = 0;
  range[1] = 10000;
}

GCalibFunc::~GCalibFunc(void){}

void GCalibFunc::SetRange(Double_t xlow, Double_t xup)
{
  range[0] = xlow;
  range[1] = xup;
}

TF1* GCalibFunc::CalibFunc(CalFuncType func_type)
{
  TF1 *fTF1;
  ostringstream oss;
  oss.str("");
  oss << f_name << f_num;
  if(func_type == kLine)
    fTF1 = new TF1(oss.str().c_str(), "pol1(0)", range[0], range[1]);
  if(func_type == kPol2)
    fTF1 = new TF1(oss.str().c_str(), "pol2(0)", range[0], range[1]);
  if(func_type == kPol3)
    fTF1 = new TF1(oss.str().c_str(), "pol3(0)", range[0], range[1]);
  if(func_type == kPol4)
    fTF1 = new TF1(oss.str().c_str(), "pol4(0)", range[0], range[1]);
  if(func_type == kPol5)
    fTF1 = new TF1(oss.str().c_str(), "pol5(0)", range[0], range[1]);
  if(func_type == k2Line)
    fTF1 = new TF1(oss.str().c_str(), TwoLine, range[0], range[1],5);
  if(func_type == kHyperbolic)
    fTF1 = new TF1(oss.str().c_str(), "[0]*x+sqrt([1]*x^2 + [2])", range[0], range[1]);
  return (TF1*)fTF1->Clone();
}

Double_t GCalibFunc::CalcResidualError(Double_t ch, Double_t che, Double_t RefEe, Double_t* par, Double_t*parE, CalFuncType func_type)
{
  Double_t fitE;
  if(func_type == kLine){
    fitE = ch*ch *parE[1]*parE[1] + parE[0]*parE[0] + che*che*par[1]*par[1];
  }
  else if(func_type == kPol2){
    fitE = parE[0]*parE[0] + ch*ch*parE[1]*parE[1] + ch*ch*ch*ch*parE[2]*parE[2] + pow( 2.0*ch*par[2] + par[1], 2 )*che*che;
  }
  else if(func_type == kPol3){
    fitE = parE[0]*parE[0] + ch*ch*parE[1]*parE[1] + pow(ch,4)*parE[2]*parE[2] + pow(ch,6)*parE[3]*parE[3] + pow( 3.0*ch*ch*par[3] + 2.0*ch*par[2] + par[1], 2 )*che*che;	  
  }
  else if(func_type == k2Line){
    if( ch < par[0] )
      fitE = ch*ch *parE[2]*parE[2] + parE[1]*parE[1] + che*che*par[2]*par[2];
    else
      fitE = ch*ch *parE[4]*parE[4] + parE[3]*parE[3] + che*che*par[4]*par[4];
  }
  else if(func_type == kHyperbolic){
    Double_t Sx2 = 2.0*ch*che;
    Double_t Sp1x2 = par[1]*ch*ch*sqrt( pow(parE[1]/par[1], 2) + Sx2*Sx2/(ch*ch) );
    Double_t p1x2Ap2 = sqrt( Sp1x2*Sp1x2 + parE[2]parE[2] );
    Double_t sqp1x2Ap2 = p1x2Ap2/(2.0*sqrt( par[1]*ch*ch + par[2] ));
    Double_t p0x = par[0]*ch*sqrt( parE[0]*parE[0]/(par[0]*par[0]) + che*che/(ch*ch) );
    fitE = p0x*p0x + sqp1x2Ap2*sqp1x2Ap2;
  }
  return sqrt(  fitE + RefEe*RefEe );
}

void GCalibFunc::SetInitialParam(TF1* fTF1, Double_t gain, Double_t offset, CalFuncType func_type)
 {
   if(func_type == kLine){
     fTF1->SetParameters(offset, gain);
   }
   else if(func_type == kPol2){
     fTF1->SetParameters(offset, gain, 0);
   }
   else if(func_type == kPol3){
     fTF1->SetParameters(offset, gain, 0, 0);
   }
   else if(func_type == k2Line){
     fTF1->SetParameters(LIN_DIV, offset, gain, offset, gain);
   }
   else if(func_type == kHyperbolic){
     Double_t g1 = 0.5, g2 = 0.501;
     Double_t A = 100;
     Double_t B = 1.0 / ( tan( atan((g1-g2)/(1+g1*g2))/2.0 ) );
     Double_t Th = atan( (B - g1)/(1+B*g1) );
     Double_t Alpha = cos(Th)*cos(Th) - B*sin(Th)*sin(Th);
     Double_t Beta = sin(Th)*cos(Th)(1+B);
     Double_t Gamma = sin(Th)*sin(Th) - B*cos(Th)*cos(Th);
     fTF1->SetParameters(Beta/Alpha, (Beta*Beta)/(Alpha*Alpha) - Gamma/Alpha, A/Alpha);
   }
   return;
}
