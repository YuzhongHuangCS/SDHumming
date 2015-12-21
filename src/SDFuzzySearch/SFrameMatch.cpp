////////////////////////////////////////////////////////////////////////////////
/**
 * @file   SFrameMatch.cpp
 * @author Wang Lei <wanglei.will@snda.com>
 * @date   Thu Aug 19 10:37:14 2010
 * 
 * @brief  
 * The frame matching fuctions
 * 
 */
////////////////////////////////////////////////////////////////////////////////

#include "SFrameMatch.h"
#include <stdio.h>
#include <math.h>
float pMatrix[800][800];
#define QRY_LEN 820
#define TPL_LEN 1200

inline float Dist2(float x,float y){
	return (float)fabs(x-y);
}

inline float GetMinDis(float* pDis,int len,int *index){
	float minDis = 10000;
    for(int i=0;i<len;i++){
		if(pDis[i]<minDis){
			minDis = pDis[i];
			*index = i;
		}
	}
	return minDis;
}

/* compute the frame-based similarity of two vector */
float SCalcDTWDistance(float* Qry, int lenQry, float* Lib, int lenLib){
	if (lenQry>QRY_LEN)
		printf("Error: Query Length too large\n");
	if (lenLib>TPL_LEN)
		printf("Error: Template Length too large\n");
	
	int r,t;
	int iMin,iMax;
	float minDist(10000);

    for(t=0;t<lenQry;t++){
		iMin = (int)ceil(0.5*t);
		iMax = (int)floor(0.5*t+lenLib-0.5*lenQry+0.5);
        if(iMax>=lenLib)
			iMax = lenLib - 1;
 
		for(r=0;r<lenLib;r++){
			if(iMin<=r && r<=iMax){
				if(t==0 || r==0 || t==1 || r==1)
					pMatrix[t][r] = 0.0;
				else{
					float Dis = Dist2(Qry[t],Lib[r]);
					pMatrix[t][r] = Min3(pMatrix[t-1][r-1]+Dis,pMatrix[t-1][r-2]+3*Dis,pMatrix[t-2][r-1]+3*Dis);
				}
			}else{
				pMatrix[t][r] = 10000;
			}
	
		}
	}

	int LibY;
    minDist = GetMinDis(pMatrix[lenQry-1],lenLib,&LibY);
	
	return minDist;
}
