/*
* SUtil.h SDHumming Util routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/

#ifndef SDHumming_SUTIL_H_
#define SDHumming_SUTIL_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "STypes.h"

/*  Compare two float value, this function is mainly used for qsort */
int _compareFloat(const void *a, const void *b);

/*
*  For reading song melody DB's associated information, such as
*  song's name, singer and so on.
*/
int SReadMelodyDBInfoFile(char* sFileName, char** &psStrRead);

/*
*  For alleviating half and double octave frequency error
*  typically, in this program, the MEDIAN_FILTER_WINDOWS_SIZE
*  is set to be five
*/
void SMedianFilter(float *&fPitchArray,int len);

#endif //SDHumming_SUTIL_H_
