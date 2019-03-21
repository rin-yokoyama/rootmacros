//setting macros for GAROOT lib
#ifndef GAROOT_SETGAROOT_H
#define GAROOT_SETGAROOT_H

#include "GAROOT/src/CalibFunctions.h"

#define FIX_PRM "GAROOT/prm/fixparam.txt"
#define SRC_LIST "GAROOT/prm/sourcelist.txt"
#define TCAL_CONF "GAROOT/prm/tcal_conf.txt"
#define SLEW_CONF "GAROOT/prm/tslew_conf.txt"
#define EFF_CONF "GAROOT/prm/Eff_conf.txt"

#define CANV_H 480.0 //height of the canvas
#define CANV_W 16.0/9.0*CANV_H //width of the canvas
#define NH_MAX 512 //maximum number of histograms
#define NF_MAX 64 //maximum number of TF1s
#define NP_MAX 5 //maximum number of peaks
#define NM_MAX  NP_MAX+2 //maximum number of markers

#define ECAL_FUNC kHyperbolic //type of the function for energy calibration
#define LIN_DIV 1200 //initial division point of the k2Line function
#define E_RES 0.003 //energy resolution
#define E_RES_MIN 0.0 //minimum of the energy resolution
#define QF_R 3 //+-range of the quick fitting (QF_R*E_RES*[center] will be the range)

#endif
