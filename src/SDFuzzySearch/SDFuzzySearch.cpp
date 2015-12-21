////////////////////////////////////////////////////////////////////////////////
/**
 * @file   SDFuzzySearch.cpp
 * @author Wang Lei <wanglei.will@snda.com>
 * @date   Thu Aug 19 10:37:14 2010
 * 
 * @brief  
 * The implementation of SNDA fuzzy search engine
 * 
 */
////////////////////////////////////////////////////////////////////////////////

#include "SDFuzzySearch.h"
#include <math.h>

/*  Compare two FrameBasedResStru items according fScore field */
int _compareDTWScore(const void *a, const void *b){
	return (((FrameBasedResStru*)a)->fScore > ((FrameBasedResStru*)b)->fScore)? 1:-1; 
}

/*  Compare two NoteBasedResStru items according fScore field */
int _compareEMDRes(const void *a, const void *b){
	return (((NoteBasedResStru*)a)->fScore > ((NoteBasedResStru*)b)->fScore)? 1:-1; 
}

/*
*  Use segmented note sequence as input to measure two melody similarity,
*  which is fast but less accurate
*/
#ifdef WIN32
	SD_SEARCH_DLL int SNoteBasedMatch(const SModel *SQBHModels,const  int nModels, SNote *Query, 
			  int QueryLen,NoteBasedResStru *myEMDResStru,int SourceLen){
#else
	int SNoteBasedMatch(const SModel *SQBHModels,const  int nModels, SNote *Query, 
			  int QueryLen,NoteBasedResStru *myEMDResStru,int SourceLen){
#endif

	int i,j;
	int nCount=0;

	int m_LenQuery=0;
	feature_t m_FeatureQuery[MAX_EMD_FEASIZE];
	float m_WeightQuery[MAX_EMD_FEASIZE];
	int m_DurQuery=0;
	
	/* for processing the query note sequence */
	SQueryNoteProcessing(SourceLen, Query, QueryLen, 
		m_FeatureQuery, m_WeightQuery, m_LenQuery, m_DurQuery);	

	/* calculate the note-based similarity against on every song's phrase */
  	for (i=0; i<nModels; i++){
		float fModelMin = (float)MAX_NUMBER;
		int nOffset=-1;
		
		/* for each phrases. */
		for (int k=0; k<SQBHModels[i].nPhraseNum; k++){
			int NotesSize=SQBHModels[i].nNoteNum-SQBHModels[i].PhrasePos[k];
			int DestLen=SGetNotesPitchLen(SQBHModels[i].sNotes,SQBHModels[i].PhrasePos[k],NotesSize);
		
			float MinVal = (float)MAX_NUMBER;
			float Dist=0.0f;
			float MinScale=0.0f;
			float ScaleFactor=0.0f;
			int DstLen;

			/* since we donot know the end point of the query, so try several times to find out the best fitting factor */
			for (j=0;j<EMD_SCALING_TIMES;j++){
				ScaleFactor =(float)(1 + EMD_SCALING_STEP*j - EMD_SCALING_OFFSET);
				DstLen = (int)((SourceLen*ScaleFactor>DestLen) ? DestLen : SourceLen*ScaleFactor); 
				Dist = SModelNoteMatching(DstLen,
					SQBHModels[i].sNotes+SQBHModels[i].PhrasePos[k],NotesSize,
					m_FeatureQuery, m_WeightQuery, m_LenQuery,m_DurQuery);

				if (MinVal>Dist) {
					MinVal=Dist;
					MinScale=ScaleFactor;
				}				
			}

			myEMDResStru[nCount].nModelID=i;
			myEMDResStru[nCount].nPhraseID=k;
			myEMDResStru[nCount].fScore=MinVal;
			myEMDResStru[nCount].fScale=MinScale;
			nCount++;
		}
	}
	/* sort the result by similarity*/
	qsort(myEMDResStru, nCount, sizeof(NoteBasedResStru),_compareEMDRes);

	return true;	
}

/*
*  Use pitch sequence as input to measure two melody similarity,
*  which is accurate but slow
*/
#ifdef WIN32
	SD_SEARCH_DLL int SFrameBasedMatch(const SModel *SQBHModels, const int nModels, 
			  float *fQueryPitchVector, int nQueryPitchVectorLen, 
			  NoteBasedResStru *myEMDResStru, int nCandidate, FrameBasedResStru *res){
#else
	int SFrameBasedMatch(const SModel *SQBHModels, const int nModels, 
			  float *fQueryPitchVector, int nQueryPitchVectorLen, 
			  NoteBasedResStru *myEMDResStru, int nCandidate, FrameBasedResStru *res){
#endif
	int i=0,j=0,k=0, m=0;
	float fModelMinDist = (float)MAX_NUMBER;
	float fDist=0.0f;
	float fSpan=0.0f,fCenter=0.0f,s_1=0.0f,s1=0.0f,s0=0.0f,fSmin=0.0f;

    float *fDesDataBuf=NULL;
	float *fSrcDataBuf = new float[nQueryPitchVectorLen];
	int nRemoveSilenceCount=nQueryPitchVectorLen;
	int MaxTemplatelen=0;

	/* only consider the survival candidates for the frame matching for re-ranking the candidates */
	for (int n=0; n<nCandidate; n++){
		i=myEMDResStru[n].nModelID;
		j=myEMDResStru[n].nPhraseID;
		float fModelMin=MAX_NUMBER+0.0f;
		int nOffset=-1; 

		fModelMinDist = (float)MAX_NUMBER;
		int NotesSize=SQBHModels[i].nNoteNum-SQBHModels[i].PhrasePos[j];
		int nDesLen=SGetNotesPitchLen(SQBHModels[i].sNotes,SQBHModels[i].PhrasePos[j],NotesSize);

		if (NULL==fDesDataBuf){
			fDesDataBuf=new float[nDesLen*2];
			MaxTemplatelen=nDesLen*2; 
		}else{
			if(nDesLen>MaxTemplatelen){
				fDesDataBuf=new float[nDesLen*2];
				MaxTemplatelen=nDesLen*2;
			}
		}

		int nFar=0;

		for (k=0;k<NotesSize;k++){
			float fVal=(float)((SQBHModels[i].sNotes[SQBHModels[i].PhrasePos[j]+k].fNoteValue-69.0f)/12.0+log(440.0f)/log(2.0f));
			int NoteDur=(int)(SQBHModels[i].sNotes[SQBHModels[i].PhrasePos[j]+k].fNoteDuration);
			for (m=0;m<NoteDur/myEMDResStru[n].fScale;m++){
				 fDesDataBuf[nFar+m]=fVal;     	
			}
			nFar+=(int)(NoteDur/myEMDResStru[n].fScale);
		}

		SPreprocessMelodyDB(fDesDataBuf,nDesLen,nRemoveSilenceCount,1); 	

		/* binary search in vertical direction for finding the best aligned parameters for key shifting */
		fSpan = 0.4f;
		fCenter = 0.0;
		int dtw_tag=1;		
 		while(fSpan>0.01){
			for (k=0;k<nRemoveSilenceCount;k++)
				fSrcDataBuf[k]=fQueryPitchVector[k]+fCenter-fSpan;	
			
			s_1=SCalcDTWDistance(fSrcDataBuf, nRemoveSilenceCount-3, fDesDataBuf, nDesLen);
			
			for (k=0;k<nRemoveSilenceCount;k++)
				fSrcDataBuf[k]=fSrcDataBuf[k]+fSpan;
			
			if (dtw_tag==1)
				s0=SCalcDTWDistance(fSrcDataBuf, nRemoveSilenceCount-3, fDesDataBuf, nDesLen);
			else
				s0=fSmin;
			
			for (k=0;k<nRemoveSilenceCount;k++)
				fSrcDataBuf[k]=fSrcDataBuf[k]+fSpan;
			
			s1=SCalcDTWDistance(fSrcDataBuf, nRemoveSilenceCount-3, fDesDataBuf,  nDesLen);

			fSmin=Min3(s_1,s0,s1);
			dtw_tag=0;
			
			if (s_1-fSmin<EPSILON3)
				fCenter=fCenter-fSpan;
			
			if (s1-fSmin<EPSILON3)
				fCenter=fCenter+fSpan;
			
			if (s0-fSmin<EPSILON3)
				fCenter=fCenter;
			
			fSpan/=2;
			fDist=fSmin;
			if (fDist<fModelMinDist)
				fModelMinDist=fDist;
		}	

		if(fModelMin>fModelMinDist){
			fModelMin=fModelMinDist;
			nOffset=SQBHModels[i].PhrasePos[j];
		}
		
		if (fDesDataBuf){
			delete[] fDesDataBuf;
			fDesDataBuf=NULL;
		}

		/* use the linear fusion of the note-based and the frame-based similarity */
		res[n].fScore=fModelMin*FUSION_RATIO+myEMDResStru[n].fScore*(1-FUSION_RATIO);
		res[n].nModelID=i;
		res[n].nOffset=nOffset;
	}  

	qsort(res,nCandidate, sizeof(FrameBasedResStru),_compareDTWScore);

    if (fSrcDataBuf){
    	delete[] fSrcDataBuf; 
		fSrcDataBuf=NULL;
	}

   	return 0;
}

