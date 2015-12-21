////////////////////////////////////////////////////////////////////////////////
/**
 * @file   SFrameMatch.h
 * @author Wang Lei <wanglei.will@snda.com>
 * @date   Thu Aug 19 10:37:14 2010
 * 
 * @brief  
 * 
 * 
 */
////////////////////////////////////////////////////////////////////////////////
/*
 * Traditional similarity measure method, such as Euclidean distance, is 
 * very sensitive to missing a data point, or having additional noise points.
 * This is particularly the case for the query by humming system. The user's 
 * input may be performed faster or slower comparing to the template stored 
 * in melody database, thus, the Euclidean distance would produce significant 
 * error. To overcome this problem, dynamic time warping (DTW) algorithm is 
 * adopted to fill the gap caused by tempo variation between each individual 
 * user's acoustic input and music score template.
*/

#ifndef SDHumming_SFRAMEMATCH_H_
#define SDHumming_SFRAMEMATCH_H_
#include "STypes.h"
#include "SSearchCommon.h"

/* compute the frame-based similarity of two vector */
float SCalcDTWDistance(float* Qry, int lenQry, float* Lib, int lenLib);


#endif //SDHumming_SFRAMEMATCH_H_