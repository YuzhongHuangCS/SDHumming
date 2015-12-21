////////////////////////////////////////////////////////////////////////////////
/**
 * @file   SSearchCommon.h
 * @author Wang Lei <wanglei.will@snda.com>
 * @date   Thu Aug 19 10:37:14 2010
 * 
 * @brief  
 * 
 * 
 */
////////////////////////////////////////////////////////////////////////////////
#ifndef SDHumming_COMMON_H_
#define SDHumming_COMMON_H_

#include "STypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*  Return the pitch frames number contained in the note sequence */
int SGetNotesPitchLen(SNote *sNotes, int nStartNote, int nNotes);

/*  For normalizing the note sequence, note shift in vertical direction */
void SNormSongNote(SNote *Notes, int NoteSize,int Len);

/*
*  For pre-processing (silence & non-speech frame removal,
*  melody contour normalization) the melody template in DB
*/
void SPreprocessMelodyDB(float* pitch_array,int& Len,int SourceLen,float Ratio);

/* For returning minimun value of the three input value */
float Min3(float in1,float in2,float in3);

/*  Compare two float value, this function is mainly used for qsort */
int _compareFloat(const void *a, const void *b);

#endif