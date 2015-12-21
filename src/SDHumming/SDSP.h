/*
* SDSP.h SDHumming Digital Signal Processing routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/

#ifndef SDHUMMING_SDSP_H_
#define SDHUMMING_SDSP_H_

#include <math.h>
#include "STypes.h"


void SProSignal(int order, int tipo, float* fCut, double* &acoef,double* &bcoef);

/* butterworth filtering */
void SFilter(float *fData,int nDataLen,double* acoef,double *bcoef,int N);

/* compute the pitch contour */
float SCalcPitchValue(float *fDataBuf, int nDataLen, float fPend,int nIndexMax, int nIndexMin, int fs);

/* compute the evergy curve */
float SGetEnergyVec(float *fData, int nLen, long nFrmSht, float *&EnergyVec, int &nEnergyLen);

void SCalcDifFunction(const float *fData,int nFrm, int nIndexMax, float fPend,float *oDataBuf);

#endif  // SDHUMMING_SDSP_H_ 
