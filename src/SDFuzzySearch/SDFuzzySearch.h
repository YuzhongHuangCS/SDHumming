////////////////////////////////////////////////////////////////////////////////
/**
 * @file   SDFuzzySearch.h
 * @author Wang Lei <wanglei.will@snda.com>
 * @date   Thu Aug 19 10:37:14 2010
 * 
 * @brief  
 * 
 * 
 */
////////////////////////////////////////////////////////////////////////////////
#ifndef SDHumming_SEARCH_H_
#define SDHumming_SEARCH_H_

#include "SFrameMatch.h"
#include "SNoteMatch.h"
#include "SSearchCommon.h"

#ifdef WIN32
	#define SD_SEARCH_DLL __declspec(dllexport)
#endif

/*
*  Use segmented note sequence as input to measure two melody similarity,
*  which is fast but less accurate
*/
#ifdef WIN32
	extern "C" int SD_SEARCH_DLL SNoteBasedMatch(const SModel *SQBHModels, const int nModels, SNote *Query, int QueryLen,NoteBasedResStru *myEMDResStru,int nLen);
#else
	int SNoteBasedMatch(const SModel *SQBHModels, const int nModels, SNote *Query, int QueryLen,NoteBasedResStru *myEMDResStru,int nLen);
#endif
/*
*  Use pitch sequence as input to measure two melody similarity,
*  which is accurate but slow
*/
#ifdef WIN32
	extern "C" int SD_SEARCH_DLL SFrameBasedMatch(const SModel *SQBHModels, const int nModels, float *m_pQueryPitchVector,
			  int m_nQueryPitchVectorLen, NoteBasedResStru *NoteBasedResStru, int nCandi, FrameBasedResStru *res);
#else
	int SFrameBasedMatch(const SModel *SQBHModels, const int nModels, float *m_pQueryPitchVector,
			  int m_nQueryPitchVectorLen, NoteBasedResStru *NoteBasedResStru, int nCandi, FrameBasedResStru *res);
#endif

#endif //SDHumming_SSEARCH_H_