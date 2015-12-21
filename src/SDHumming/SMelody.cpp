/*
* SMelody.cpp SDHumming Melody Extraction routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/


#include "SDSP.h"
#include "SMelody.h"
#include <math.h>
#include <stdio.h>

/* extract pitch contour of the input sound file */
int SPitchExtraction(SWaveDataStru mySWaveDataStru, float pendiente, float fmin, float fmax, 
					 float fcutinf, float fcutsup, float*& fPitchData, int &nPitchLen){

	int fs;                           /* sampling rate */
	int nDataLen;                     /* total samples */ 
	int nFrmShtLen;                   /* samples for frame shift */
	int nFrmWinLen;                   /* samples for frame windows */
	int nIndexMax, nIndexMin;         
	int nFrameSpan;                   
	int nTotalFrms;                   /* total frames */
	float* fCutFreq=new float[2];     

	fs = mySWaveDataStru.fs;
	nDataLen = mySWaveDataStru.nDataLen;
	nFrmWinLen = (int)floor(fs*FRAME_LEN_MS/1000.0f)-1;
	nFrmShtLen = (int)floor(fs*FRAME_SHT_MS/1000.0f);
	nIndexMax = (int)ceil(fs/fmin);
	nIndexMin = (int)floor(fs/fmax);
	nFrameSpan = nFrmWinLen + nIndexMax + 1;
	nTotalFrms = (int)floor((float)((nDataLen-(nFrameSpan+nFrmShtLen))/nFrmShtLen))+1;
	
	/* compute energy curve */
	float *pEnergy=NULL;
	int nFrmNum=0;
	float fMean=SGetEnergyVec(mySWaveDataStru.fDataBuf, mySWaveDataStru.nDataLen,nFrmShtLen,pEnergy,nFrmNum);
	
	/* butterworth filtering, order 4 */
	double* acoef=NULL;
	double* bcoef=NULL;
	fCutFreq[0] = fcutinf/fs;
	fCutFreq[1] = fcutsup/fs;
	SProSignal(4, 2, fCutFreq,acoef,bcoef);
	SFilter(mySWaveDataStru.fDataBuf, mySWaveDataStru.nDataLen,acoef,bcoef,9);
	delete[] acoef;
	delete[] bcoef;

	int i = 0;
	/* use the energy to detect the silence, the silence frame is not considered */
	float fEnergyThres = 0.3f * fMean; /* energy threshold */
	fPitchData=new float[nTotalFrms];
	nPitchLen=nTotalFrms;
	while (i < nTotalFrms){
		if (pEnergy[i] > fEnergyThres){
			float *fIndexData=new float[nIndexMax+1];
			SCalcDifFunction(mySWaveDataStru.fDataBuf+nFrmShtLen*i,nFrmWinLen, nIndexMax, pendiente,fIndexData);
			fPitchData[i]=SCalcPitchValue(fIndexData, nIndexMax, pendiente,nIndexMax, nIndexMin, fs);
			delete[] fIndexData;
		}
		else{
			fPitchData[i]=0.0f;
		}
		i++;
	}
	delete[] pEnergy;
	return 0;
}

/* normalize the input data */
void NormalizeData(float *data, int nLen, float max){
	float fMaxTemp=-1;
	long ind;
	int i=0;
	for( i=0;i<nLen;i++){
		if(fMaxTemp<fabsf(data[i])){
			fMaxTemp=fabsf(data[i]);
			ind=i;	
		}
	}
	for( i=0;i<nLen;i++){
		data[i]*=max / fMaxTemp;
	}
}

/* read wave data from file */
int WaveRead(char* filename, SWaveDataStru &mySWaveDataStru){
	WAVE_HEADER szHeader;
	int nRead=0;
	int nLen=0;
	short *pBuffer=NULL;
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		return ERROR_CODE_FILE_CANNOT_OPEN;
	}
	fseek(fp, 0, SEEK_END);
	nLen = (ftell(fp)-44)/2;
	pBuffer = new short[nLen];
	if (!pBuffer) {
		return ERROR_CODE_CANNOT_ALLOC_MEM;
	}

	fseek(fp, 0, SEEK_SET);
	nRead=fread(&szHeader, sizeof(WAVE_HEADER), 1, fp);
	nRead=fread(pBuffer, sizeof(short), nLen, fp);
	fclose(fp);

	mySWaveDataStru.fs=szHeader.SamplePerSec;
	mySWaveDataStru.nDataLen=nLen;
	mySWaveDataStru.fDataBuf=new float[nLen];
	if (!mySWaveDataStru.fDataBuf) {
		return ERROR_CODE_CANNOT_ALLOC_MEM;
	}

	for(int i=0;i<nLen;i++){
		mySWaveDataStru.fDataBuf[i]=pBuffer[i]/32768.0f;
	}

	NormalizeData(mySWaveDataStru.fDataBuf, mySWaveDataStru.nDataLen,0.9f);
	delete[] pBuffer;
	return 0;
}

/* extract pitch contour of the input sound file */
int SPitchContourExtraction(char* filename,float*& fPitchData,int &nFrm){
	SWaveDataStru mySWaveDataStru;
	if(WaveRead(filename,mySWaveDataStru)!=0){
		printf("Error on WaveRead\n");
		return -1;
	}
	mySWaveDataStru.fs=8000;
	SPitchExtraction(mySWaveDataStru, 0.0015f, MIN_PITCH_VALUE, MAX_PITCH_VALUE, 50, 500,fPitchData,nFrm);
	delete[] mySWaveDataStru.fDataBuf;
	return 0;
} 

/* compute the mean value of the input array, and the zero value is not used */
float GetMean(float *fArray, int nLen){
	float RetVal=0.0f;
	int nCountValid=0;
	float fSum=0.0f;
	for(int i=0;i<nLen;i++){
		if(fArray[i]>0){
			fSum+=fArray[i];
			nCountValid++;
		}
	}
	if(nCountValid>0)
		RetVal=fSum/nCountValid;
	else
		RetVal=0;
	return RetVal;
}

/* smooth and normalize the input pitch vector */
void SProcessQuery(float* fPitchArray,int& Len){
	int i;
	float MeanVal;
	
    for (i=1;i<Len-1;i++){
		if (fPitchArray[i-1]<2 && fPitchArray[i]>2 && fPitchArray[i+1]<2)
			fPitchArray[i]=0;
	}
	
	int nCountFrm=0;
	for (i=0;i<Len-5;i++){
		if (i%5==0){
			fPitchArray[i/5]=GetMean(fPitchArray+i,5);	
			nCountFrm++;
		}
	}
	
	Len=nCountFrm;    

	float LastVal=0.0;
	for (i=1;i<Len-1;i++){
		if (fPitchArray[i-1]<2 && fabs(fPitchArray[i]-LastVal)>0.3*LastVal && fPitchArray[i+1]<2)
			fPitchArray[i]=0;

		if (fPitchArray[i]>2)
			LastVal=fPitchArray[i];		
	}

	/* remove the silence frame*/
	float temppitch=0;
 	int vadCount=0;
	int nStartTag=0;
	int nStart=0;
	for (i=0;i<Len;i++){ 
		if(nStartTag==0){
			if(fPitchArray[i]<2)
				nStart=i;
			else
				nStartTag=1;	
		}

		if (fPitchArray[i]>2){
			fPitchArray[i]=(float)(log10(fPitchArray[i])/log10(2.0f));
			if(i>3)
				temppitch=GetMean(fPitchArray+i-3,3);
			else
	            temppitch=fPitchArray[i]; 
             vadCount=0;
		}else{
 			vadCount++;
			if (temppitch>0)
				fPitchArray[i]=temppitch;
		}
	}
		
	for(i=0;i<Len-nStart-1;i++){
		fPitchArray[i]=fPitchArray[i+nStart+1];
	}
	Len-=(nStart+1);

	MeanVal=0;
	int VadSize=0;
	for (i=0;i<Len;i++){
         if (fPitchArray[i]>6.3){
			 MeanVal+=fPitchArray[i];
			 VadSize++;
		 }
	}
	
	float me=0.0f;
	if(VadSize<1){
		Len=0;
		return ;
	}
	else
		me=MeanVal/VadSize;

	for (i=0; i<Len; i++){
		fPitchArray[i]=fPitchArray[i]-me+PITCH_NORMALIZE_VALUE;
		if (fPitchArray[i]>8)
			fPitchArray[i]=fPitchArray[i]-1;  
		if (fPitchArray[i]<6.35)
			fPitchArray[i]=fPitchArray[i]+1;
	}
}

/* note transcription, the inpyt is pitch vector, and the note sequence is generated */
int STranscribeQueryNote(float* fPitchArray,int& Len,SNote *&Query, int &nNoteLen){
	int i;
	int startindex=0; 

	for (i=1;i<Len-1;i++){
		/* remove the pitch jitter and the transition pitch value */
		if (((fPitchArray[i]-fPitchArray[i-1])>0.01 && (fPitchArray[i+1]-fPitchArray[i])>0.01)
	         || ((fPitchArray[i]-fPitchArray[i-1])<-0.01 && (fPitchArray[i+1]-fPitchArray[i])<-0.01)){
			float DistPre=(float)fabs(fPitchArray[i]-fPitchArray[i-1]);
			float DistAfter=(float)fabs(fPitchArray[i]-fPitchArray[i+1]);
	        if (DistPre<DistAfter)
	     		fPitchArray[i]=fPitchArray[i-1];
			else
		    	fPitchArray[i]=fPitchArray[i+1];
		}
	}

    /* combine the note by significant pitch change */
	for (i=0;i<Len;i++){
		if (fabs(fPitchArray[i+1]-fPitchArray[i])>0.05 || i==Len-1) {
			int nth=i+1-startindex;
			float *fTempMedian=new float[nth];
			memcpy(fTempMedian,fPitchArray+startindex,sizeof(float)*nth);
			
			if(nth>2){
				qsort(fTempMedian,nth, sizeof(float),_compareFloat);
				for(int j=0;j<nth;j++){
					fPitchArray[startindex+j]=fTempMedian[(nth+1)/2];
				}
			}
			startindex=i+1; 
			delete[] fTempMedian;
		} 
	}

	SNoteSequenceStru *retVal=new SNoteSequenceStru;
	retVal->Next=NULL;
	SNoteSequenceStru *CurrentNote=retVal;
	SNote Note;
	float OldPitch;
	Note.fNoteValue=fPitchArray[0]; 
	OldPitch=Note.fNoteValue;
	Note.fNoteDuration=1;

	/* post processing the note sequence */
	for (i=1;i<Len;i++){
		if (!FLOAT_EQUAL(fPitchArray[i],OldPitch)){
			if (Note.fNoteDuration<=2){
				float PrePitch;
      			if (NULL==retVal->Next)
					PrePitch=(float)MAX_NUMBER;
				else{
					SNoteSequenceStru *tmp=retVal;
					while(NULL!=tmp->Next){
						tmp=tmp->Next;
					}
					PrePitch=tmp->fNote.fNoteValue;
				}
				
				float OldPitch=fPitchArray[i];
				
				/* combine with the following note */
				if (fabs(Note.fNoteValue-PrePitch)<fabs(Note.fNoteValue-OldPitch)){
					SNoteSequenceStru *tmp=retVal;
					while(NULL!=tmp->Next){
						tmp=tmp->Next;
					}
					
					tmp->fNote.fNoteDuration+=Note.fNoteDuration;
					float TempVal=tmp->fNote.fNoteValue;
					for(int j=i-(int)Note.fNoteDuration;j<i;j++){
						fPitchArray[j]=TempVal;
					}
					Note.fNoteValue=fPitchArray[i];
					Note.fNoteDuration=1;
				}else{/* combine with the previous note */
					Note.fNoteValue=fPitchArray[i];
					for(int j=i-(int)Note.fNoteDuration;j<i;j++){
						fPitchArray[j]=Note.fNoteValue;
					}
					Note.fNoteDuration+=1;
				}
			}else{
				/* new note begin */
				SNoteSequenceStru *tmpNotes=new SNoteSequenceStru;
				tmpNotes->fNote=Note;
				tmpNotes->Next=NULL;
				CurrentNote->Next=tmpNotes;
				CurrentNote=CurrentNote->Next;
				Note.fNoteValue=fPitchArray[i];
				Note.fNoteDuration=1;
				nNoteLen++;
			}	
			OldPitch=fPitchArray[i];
		}else
			Note.fNoteDuration+=1;
	}
	SNoteSequenceStru *tmpNotes=new SNoteSequenceStru;
	tmpNotes->fNote=Note;
	tmpNotes->Next=NULL;
	CurrentNote->Next=tmpNotes;
	nNoteLen++;

	Query=new SNote[nNoteLen];
	nNoteLen=0;	

	/* convert the note sequence into MIDI format */
	CurrentNote=retVal->Next;
	if (Note.fNoteValue>0 && Note.fNoteValue<10){
		while(NULL!=CurrentNote){
			CurrentNote->fNote.fNoteValue=(float)(12.0f*(CurrentNote->fNote.fNoteValue-log(440.0f)/log(2.0f))+69.0f);
			Query[nNoteLen].fNoteValue=CurrentNote->fNote.fNoteValue;
			Query[nNoteLen].fNoteDuration=CurrentNote->fNote.fNoteDuration;
			CurrentNote=CurrentNote->Next;
			nNoteLen++;
		}
	}

	return 0;
}