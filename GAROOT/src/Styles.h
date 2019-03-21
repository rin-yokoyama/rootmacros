#ifndef GAROOT_STYLES_H
#define GAROOT_STYLES_H

#include <TAttLine.h>
#include <TAttFill.h>
#include <TAttMarker.h>

//defines Fit and Histogram styles.
//TAttLine(color, style, width), TAttFill(color, style)
const TAttLine gGFitLine(kRed, 1, 2); //the style of fitting functions
const TAttLine gGBGLine(kBlack, 1, 1); //the style of background part of fitting functions

//styles of histograms
const TAttLine gGHistLine(kBlue+3, 1, 1);
const TAttFill gGHistFill(kBlue+3, 0);

const TAttMarker gPSMarker(kRed, 23, 1); // the style of markers for peak search.
const TAttMarker gGraph(kRed, 8, 0.8); //the style of TGraphs

#endif
