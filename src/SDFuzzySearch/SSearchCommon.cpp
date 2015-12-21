////////////////////////////////////////////////////////////////////////////////
/**
 * @file   SSearchCommon.cpp
 * @author Wang Lei <wanglei.will@snda.com>
 * @date   Thu Aug 19 10:37:14 2010
 * 
 * @brief  
 * the common functions of fuzzy searching
 * 
 */
////////////////////////////////////////////////////////////////////////////////

#include "SSearchCommon.h"
/*  Return the pitch frame points contained in the note sequence */
int SGetNotesPitchLen(SNote *sNotes, int nStartNote, int nNotes){
	int iTotalPitch=0;
	for(int i=nStartNote;i<nStartNote+nNotes;i++){
		iTotalPitch+=(int)sNotes[i].fNoteDuration;
	}
	return iTotalPitch;
}

/*  Compare two float value, this function is mainly used for qsort */
int _compareFloat(const void *a, const void *b){
	return (*(float*)a > *(float*)b)? 1:-1; 
}


/*  For normalizing the note sequence, note shift in vertical direction */
void SNormSongNote(SNote *Notes, int iNoteSize,int iLen){
	int i;
	int iDesLen=SGetNotesPitchLen(Notes,0, iNoteSize);
 	
	int iFirstLen;
	if (iDesLen>iLen){
		iFirstLen=iLen;
	}else{
		iFirstLen=iDesLen;
	}

	float fNoteCount=0.0f;
	float fSumNotesValue=0.0f;
	for (i=0;i<iNoteSize;i++){
		fNoteCount+=Notes[i].fNoteDuration;
		fSumNotesValue+=Notes[i].fNoteValue*Notes[i].fNoteDuration;
		if (fNoteCount>iFirstLen)
			break;
	}

	float fMeanNotesValue=fSumNotesValue/fNoteCount;
    for (i=0;i<iNoteSize;i++) 
		Notes[i].fNoteValue=Notes[i].fNoteValue-fMeanNotesValue+NOTE_NORMALIZE_VALUE;
}

/*
*  For pre-processing (silence & non-speech frame removal,
*  melody contour normalization) the melody template in DB
*/
void SPreprocessMelodyDB(float* fPitchArray, int& iLen, int SourceLen,float Ratio){
	int j;
	int iRemoveSilCount=0;
	
	/*Remove the silence or non-speech frame*/
	for (j=0;j<iLen;j++){
		if (fPitchArray[j]>2){
			fPitchArray[iRemoveSilCount++]=fPitchArray[j];
		}
	}

	if (SourceLen>0){
		int iDesLen=(int)(SourceLen*Ratio);
		if (iDesLen<iLen) iLen=iDesLen;
	}else{
    	iLen=iRemoveSilCount;
	}
	
	/*Pitch array normalization*/
	float fSum=0.0f;
	float fTargetAverage=0.0f;
	for (j=0;j<iLen;j++){
		fSum=fSum+fPitchArray[j];
	}
	fTargetAverage=fSum/iLen;
		
	for (j=0;j<iLen;j++){
		fPitchArray[j]=fPitchArray[j] - fTargetAverage + PITCH_NORMALIZE_VALUE;
	}
}
inline float Min2(float x,float y){
	return x>y?y:x;	
}


float Min3(float in1,float in2,float in3){
	return Min2(in3,Min2(in1,in2));
}