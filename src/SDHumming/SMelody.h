/*
* SMelody.h SDHumming Melody Extraction routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/


#ifndef SDHUMMING_SMELODY_H_
#define SDHUMMING_SMELODY_H_

#include "SUtil.h"
#include "STypes.h"

/* extract pitch contour of the input sound file */
int SPitchContourExtraction(char* szInFile,float*& fPitchData,int &nFrm);

/* smooth and normalize the input pitch vector */
void SProcessQuery(float* fPitchArray,int& Len);

/* note transcription, the inpyt is pitch vector, and the note sequence is generated */
int STranscribeQueryNote(float* fPitchArray,int& Len,SNote *&Query, int &nNoteLen);
#endif //SDHUMMING_SMELODY_H_