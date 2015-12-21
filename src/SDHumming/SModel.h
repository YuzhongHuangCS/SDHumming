/*
* SModel.h SDHumming Melody Database routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/


#ifndef SDHUMMING_SMODEL_H_
#define SDHUMMING_SMODEL_H_

#define MAX_PITCH_THRE 450
#define MAX_SEG_NUM 3000

#include "SUtil.h"

/*  load song model database into memory from file */
int SLoadModel(char* ModelFile, SModel *&SQBHModels, int &nModels);

#endif //SDHUMMING_SMODEL_H_
