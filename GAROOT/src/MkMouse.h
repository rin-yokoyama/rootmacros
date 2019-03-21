#ifndef GAROOT_MKMOUSE_H
#define GAROOT_MKMOUSE_H

#include <TMarker.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TPad.h>
#include <RQ_OBJECT.h>

//This class draws marker when it gets a mouse click signal from canvas.
class MkMouse
{
  RQ_OBJECT("MkMouse")

 public:
  TObjArray* fMarkerArray;
  MkMouse();
  virtual ~MkMouse(void);
  void SetMarker(Int_t event, Int_t px, Int_t py, TObject *sel);
  void Clear(void); //clears marker positions and mknum.
  TObjArray* GetArray(void) const { return fMarkerArray; };
};

#endif
