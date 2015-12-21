/*
* SUtil.cpp  SDHumming Util routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/

#include "SUtil.h"

/*  Compare two float value, this function is mainly used for qsort */
int _compareFloat(const void *a, const void *b){
	return (*(float*)a > *(float*)b)? 1:-1; 
}



/* For removing the '\r' '\n' char at the end of a string */
void chomp( char *pszString ){
	int nLen = strlen( pszString );
	while ( pszString[nLen-1] == '\r' || pszString[nLen-1] == '\n' ) 
		--nLen;
	pszString[nLen] = 0;
}

/*
*  For reading song melody DB's associated information, such as
*  song's name, singer and so on.
*/
int SReadMelodyDBInfoFile(char* sFileName, char** &psStrRead){
	int iTotalFile=0;
	FILE* fp = fopen(sFileName,"r");
	if(NULL==fp){
		return ERROR_CODE_FILE_CANNOT_OPEN;
	}
	char szTmpStr[LINEREAD];
	if(fgets(szTmpStr,sizeof(char)*LINEREAD,fp)!=NULL){
		iTotalFile=atoi(szTmpStr);
	}else{
		return ERROR_CODE_MODELINFO_WRONG_FORMAT;
	}

	psStrRead=new char*[iTotalFile];
	for(int i=0;i<iTotalFile;i++){
		psStrRead[i]=new char[LINEREAD];
		fgets(psStrRead[i],sizeof(char)*LINEREAD,fp);
		chomp(psStrRead[i]);
	}
	fclose(fp);
	return iTotalFile;
}

/*
*  For alleviating half and double octave frequency error
*  typically, in this program, the MEDIAN_FILTER_WINDOWS_SIZE
*  is set to be five
*/
void SMedianFilter(float *&fPitchArray,int iLen){
	int i,j;
	float fPitchArrayBak[MEDIAN_FILTER_WINDOWS_SIZE];

	for (i=0;i<iLen-MEDIAN_FILTER_WINDOWS_SIZE;i++)	{
		for (j=i;j<i+MEDIAN_FILTER_WINDOWS_SIZE;j++)
			fPitchArrayBak[j-i]=fPitchArray[j];

		qsort(fPitchArrayBak,MEDIAN_FILTER_WINDOWS_SIZE, sizeof(float),_compareFloat);
		fPitchArray[i + MEDIAN_FILTER_HALF_WINDOWS_SIZE] = fPitchArrayBak[MEDIAN_FILTER_HALF_WINDOWS_SIZE];
	}
	return ;
}


