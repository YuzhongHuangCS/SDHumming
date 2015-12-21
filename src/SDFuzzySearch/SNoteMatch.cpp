////////////////////////////////////////////////////////////////////////////////
/**
 * @file   SNoteMatch.cpp
 * @author Wang Lei <wanglei.will@snda.com>
 * @date   Thu Aug 19 10:37:14 2010
 * 
 * @brief  
 * The note matching fuctions
 * 
 */
////////////////////////////////////////////////////////////////////////////////

/*
    Partially based on Yossi Rubner code:
    =========================================================================
    emd.c

    Last update: 3/14/98

    An implementation of the Earth Movers Distance.
    Based of the solution for the Transportation problem as described in
    "Introduction to Mathematical Programming" by F. S. Hillier and 
    G. J. Lieberman, McGraw-Hill, 1990.

    Copyright (C) 1998 Yossi Rubner
    Computer Science Department, Stanford University
    E-Mail: rubner@cs.stanford.edu   URL: http://vision.stanford.edu/~rubner
    ==========================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "SNoteMatch.h"

float SModelNoteMatching(int iFirstNum, SNote *OriNotes, int NoteSize, 
											 feature_t *m_FeatureQuery, float *m_WeightQuery, int m_LenQuery, int m_DurQuery){
	CEmdWrapper m_emdComp;
	feature_t m_pTempFea[MAX_EMD_FEASIZE];
	float m_pTempWeight[MAX_EMD_FEASIZE];

	SNote *Notes=new SNote[NoteSize];
	memcpy(Notes,OriNotes,NoteSize*sizeof(SNote));
	int i,count=0;  
	
	SNormSongNote(Notes,NoteSize,iFirstNum);	

	//Caculate The scale factor to SCALE THE TEMPLATE
	float ScaleFactor;
	ScaleFactor=m_DurQuery/(float)iFirstNum;
	
	float fSum=0.0f;
	if (iFirstNum>20){
		for (i=0;i<NoteSize;i++){
			count++;
			fSum+=Notes[i].fNoteDuration;
			if (fSum>iFirstNum)
		        break;
		}
	}
	iFirstNum=count;

	//Make sure the Note collected is less than the note size
	if (iFirstNum>NoteSize) iFirstNum=NoteSize;

	//Propose the template
	if(iFirstNum>MAX_EMD_FEASIZE)
		printf(" %d\n",iFirstNum);
	
	int DurTemplate=SGetNotesPitchLen(Notes,0, iFirstNum);
	float Far(0),FarCnt(1);
	for (i=0;i<iFirstNum;i++){
		m_pTempFea[i].px=Far;
		m_pTempFea[i].py=(Notes[i].fNoteValue-35)*7;
		Far += Notes[i].fNoteDuration*ScaleFactor;
		m_pTempWeight[i] = Notes[i].fNoteDuration*FarCnt*ScaleFactor;
		FarCnt *= FORGET_FACTOR;
	}

	if (m_LenQuery>=MAX_EMD_FEASIZE)
		printf("Warning: Query feature length reach max\n");
	if (iFirstNum>=MAX_EMD_FEASIZE)
		printf("Warning: Template feature length reach max\n");
 	 
	signature_t s1 = { m_LenQuery, m_FeatureQuery, m_WeightQuery };
	signature_t	s2 = { iFirstNum, m_pTempFea, m_pTempWeight };

	for(int x=0;x<m_LenQuery;x++){
		if(m_FeatureQuery[x].py<60)
			m_FeatureQuery[x].py=60.0f;
	}

	float       e; 
	flow_t      flow[MAX_FLOW_NUM];
	int         flowSize;
	
	//apply emd algorithm to compute EMD distance
	e = m_emdComp.emd(&s1,&s2, flow, &flowSize);
    e += (float)(fabs(m_LenQuery-iFirstNum+0.0f));

	delete[] Notes;
	return (float)fabs(e);
}


float SQueryNoteProcessing(int iFirstNum, SNote *OriNotes, int NoteSize,
											  feature_t *m_FeatureQuery, float *m_WeightQuery, int &m_LenQuery, int &m_DurQuery){
	SNote *Notes=new SNote[NoteSize];
	memcpy(Notes,OriNotes,NoteSize*sizeof(SNote));
	int i,count=0;  
	
	SNormSongNote(Notes,NoteSize,iFirstNum);	

	//Caculate The scale factor to SCALE THE TEMPLATE	
	float fSum=0.0f;
	if (iFirstNum>20){
		for (i=0;i<NoteSize;i++){
			count++;
			fSum+=Notes[i].fNoteDuration;
			if (fSum>iFirstNum)
		        break;
		}
	}
	iFirstNum=count;

	//Make sure the Note collected is less than the note size
	if (iFirstNum>NoteSize) iFirstNum=NoteSize;

	m_LenQuery = iFirstNum;
	float fOffset=0.0f;
	float fOffsetCnt=1.0f;
	for (i=0;i<iFirstNum;i++){
		m_FeatureQuery[i].px = fOffset;
		m_FeatureQuery[i].py = (Notes[i].fNoteValue-35) * 7;
		fOffset += Notes[i].fNoteDuration;
		m_WeightQuery[i] = Notes[i].fNoteDuration * fOffsetCnt;
 		fOffsetCnt *= FORGET_FACTOR;
	}

	m_DurQuery=SGetNotesPitchLen(Notes,0, iFirstNum);
	delete[] Notes;
	return 0;
}

float CEmdWrapper::emd(signature_t *Signature1, signature_t *Signature2, 
					   flow_t *Flow, int *FlowSize){
	int itr=0;
	float totalCost;
	float w;
	node2_t *XP;
	flow_t *FlowP;
	node1_t U[MAX_SIG_SIZE1], V[MAX_SIG_SIZE1];

	w = init(Signature1, Signature2);

	if (_n1 > 1 && _n2 > 1){/* IF _n1 = 1 OR _n2 = 1 THEN WE ARE DONE */
		for (itr = 1; itr < MAX_ITERATIONS; itr++){
			/* FIND BASIC VARIABLES */
			findBasicVariables(U, V);
			
			/* CHECK FOR OPTIMALITY */
			if (isOptimal(U, V)){
				//printf("itr: %d\n",itr);
				break;
			}
			
			/* IMPROVE SOLUTION */
			newSol();

		}
		
		if (itr == MAX_ITERATIONS)
			fprintf(stderr, "emd: Maximum number of iterations has been reached (%d)\n",
			MAX_ITERATIONS);
    }
	
	/* COMPUTE THE TOTAL FLOW */
	totalCost = 0;
	if (Flow != NULL)
		FlowP = Flow;
	for(XP=_X; XP < _EndX; XP++){
		if (XP == _EnterX)  /* _EnterX IS THE EMPTY SLOT */
			continue;
		if (XP->i == Signature1->n || XP->j == Signature2->n)  /* DUMMY FEATURE */
			continue;
		
		if (XP->val == 0)  /* ZERO FLOW */
			continue;
		
		totalCost += XP->val * _C[XP->i][XP->j];
		if (Flow != NULL){
			FlowP->from = XP->i;
			FlowP->to = XP->j;
			FlowP->amount = XP->val;
			FlowP++;
		}
    }
	
	if (Flow != NULL)
		*FlowSize = FlowP-Flow;
	
	/* RETURN THE NORMALIZED COST == EMD */
	return (float)(totalCost / w);
}

float CEmdWrapper::init(signature_t *Signature1, signature_t *Signature2){
	int i, j;
	float sSum, dSum, diff;
	feature_t *P1, *P2;
	float S[MAX_SIG_SIZE1], D[MAX_SIG_SIZE1];
	
	_n1 = Signature1->n;
	_n2 = Signature2->n;
	
	if (_n1 > MAX_SIG_SIZE || _n2 > MAX_SIG_SIZE){
		fprintf(stderr, "emd: Signature size is limited to %d\n", MAX_SIG_SIZE);
		exit(1);
    }
	
	_maxC = 0;
	
	float DeltaX,DeltaY;
	for(i = 0, P1 = Signature1->Features; i < _n1; i++, P1++){
		for(j = 0, P2 = Signature2->Features; j < _n2; j++, P2++){
			DeltaX=(float)fabs(P1->px-P2->px);
			DeltaY=(float)fabs(P1->py-P2->py);
			
			DeltaX=DeltaX>110?110:DeltaX;
			DeltaY=DeltaY>350?350:DeltaY;
			
			int IndexX=(int)((DeltaX+0.25)/5);
			int IndexY=(int)((DeltaY+0.25)/5);
			
			int Index=IndexX*71+IndexY;
			_C[i][j]=(float)sqrt(DeltaX*DeltaX+DeltaY*DeltaY);
			
			if (_C[i][j] > _maxC)
				_maxC = _C[i][j];
		}
	}

	memcpy(S,Signature1->Weights,sizeof(float)*_n1);
	memcpy(D,Signature2->Weights,sizeof(float)*_n2);
	
	sSum = 0.0;
	for(i=0; i < _n1; i++){
		sSum += Signature1->Weights[i];
		_RowsX[i] = NULL;
	}
	
	dSum = 0.0;
	for(j=0; j < _n2; j++){
		dSum += Signature2->Weights[j];
		_ColsX[j] = NULL;
	}
	
	diff = sSum - dSum;
	if (fabs(diff) >= EPSILON * sSum){
		if (diff < 0.0){
			memset(_C[_n1],0,sizeof(float)*_n2);
			S[_n1] = -diff;
			_RowsX[_n1] = NULL;
			_n1++;
		}else{
			for (i=0; i < _n1; i++)
				_C[i][_n2] = 0;
			D[_n2] = diff;
			_ColsX[_n2] = NULL;
			_n2++;
		}
	}
	
	memset(_IsX[0],0,MAX_SIG_SIZE1*MAX_SIG_SIZE1);
	
	_EndX = _X;
	
	_maxW = sSum > dSum ? sSum : dSum;
	
	russel(S, D);
	
	_EnterX = _EndX++;  /* AN EMPTY SLOT (ONLY _n1+_n2-1 BASIC VARIABLES) */
	
	return sSum > dSum ? dSum : sSum;
}

void CEmdWrapper::findBasicVariables(node1_t *U, node1_t *V){
	int i, j, found;
	int UfoundNum, VfoundNum;
	node1_t u0Head, u1Head, *CurU, *PrevU;
	node1_t v0Head, v1Head, *CurV, *PrevV;
	
	/* INITIALIZE THE ROWS LIST (U) AND THE COLUMNS LIST (V) */
	u0Head.Next = CurU = U;
	for (i=0; i < _n1; i++){
		CurU->i = i;
		CurU->Next = CurU+1;
		CurU++;
    }
	(--CurU)->Next = NULL;
	u1Head.Next = NULL;
	
	CurV = V+1;
	v0Head.Next = _n2 > 1 ? V+1 : NULL;
	for (j=1; j < _n2; j++){
		CurV->i = j;
		CurV->Next = CurV+1;
		CurV++;
    }
	(--CurV)->Next = NULL;
	v1Head.Next = NULL;
	
	/* THERE ARE _n1+_n2 VARIABLES BUT ONLY _n1+_n2-1 INDEPENDENT EQUATIONS,
	SO SET V[0]=0 */
	V[0].i = 0;
	V[0].val = 0;
	v1Head.Next = V;
	v1Head.Next->Next = NULL;
	
	/* LOOP UNTIL ALL VARIABLES ARE FOUND */
	UfoundNum=VfoundNum=0;
	while (UfoundNum < _n1 || VfoundNum < _n2){
		found = 0;
		if (VfoundNum < _n2){
			/* LOOP OVER ALL MARKED COLUMNS */
			PrevV = &v1Head;
			for (CurV=v1Head.Next; CurV != NULL; CurV=CurV->Next){
				j = CurV->i;
				/* FIND THE VARIABLES IN COLUMN j */
				PrevU = &u0Head;
				for (CurU=u0Head.Next; CurU != NULL; CurU=CurU->Next){
					i = CurU->i;
					if (_IsX[i][j]){
						/* COMPUTE U[i] */
						CurU->val = _C[i][j] - CurV->val;
						/* ...AND ADD IT TO THE MARKED LIST */
						PrevU->Next = CurU->Next;
						CurU->Next = u1Head.Next != NULL ? u1Head.Next : NULL;
						u1Head.Next = CurU;
						CurU = PrevU;
					}
					else
						PrevU = CurU;
				}
				PrevV->Next = CurV->Next;
				VfoundNum++;
				found = 1;
			}
		}
		if (UfoundNum < _n1){
			/* LOOP OVER ALL MARKED ROWS */
			PrevU = &u1Head;
			for (CurU=u1Head.Next; CurU != NULL; CurU=CurU->Next){
				i = CurU->i;
				/* FIND THE VARIABLES IN ROWS i */
				PrevV = &v0Head;
				for (CurV=v0Head.Next; CurV != NULL; CurV=CurV->Next){
					j = CurV->i;
					if (_IsX[i][j]){
						/* COMPUTE V[j] */
						CurV->val = _C[i][j] - CurU->val;
						/* ...AND ADD IT TO THE MARKED LIST */
						PrevV->Next = CurV->Next;
						CurV->Next = v1Head.Next != NULL ? v1Head.Next: NULL;
						v1Head.Next = CurV;
						CurV = PrevV;
					}
					else
						PrevV = CurV;
				}
				PrevU->Next = CurU->Next;
				UfoundNum++;
				found = 1;
			}
		}
		if (! found){
			fprintf(stderr, "emd: Unexpected error in findBasicVariables!\n");
			fprintf(stderr, "This typically happens when the EPSILON defined in\n");
			fprintf(stderr, "emd.h is not right for the scale of the problem.\n");
			exit(1);
		}
    }
}

void CEmdWrapper::newSol(){
    int i, j, k;
    float xMin;
    int steps;
    node2_t *Loop[2*MAX_SIG_SIZE1], *CurX, *LeaveX;
	
    /* ENTER THE NEW BASIC VARIABLE */
    i = _EnterX->i;
    j = _EnterX->j;
    _IsX[i][j] = 1;
    _EnterX->NextC = _RowsX[i];
    _EnterX->NextR = _ColsX[j];
    _EnterX->val = 0;
    _RowsX[i] = _EnterX;
    _ColsX[j] = _EnterX;
	
    /* FIND A CHAIN REACTION */
    steps = findLoop(Loop);
	
    /* FIND THE LARGEST VALUE IN THE LOOP */
    xMin = (float)EMD_INFINITY;
    for (k=1; k < steps; k+=2){
		if (Loop[k]->val < xMin){
			LeaveX = Loop[k];
			xMin = Loop[k]->val;
		}
	}
	
    /* UPDATE THE LOOP */
    for (k=0; k < steps; k+=2){
		Loop[k]->val += xMin;
		Loop[k+1]->val -= xMin;
	}
	
    /* REMOVE THE LEAVING BASIC VARIABLE */
    i = LeaveX->i;
    j = LeaveX->j;
    _IsX[i][j] = 0;
    if (_RowsX[i] == LeaveX){
		_RowsX[i] = LeaveX->NextC;
	}else{
		for (CurX=_RowsX[i]; CurX != NULL; CurX = CurX->NextC){
			if (CurX->NextC == LeaveX){
				CurX->NextC = CurX->NextC->NextC;
				break;
			}
		}
	}
	
	if (_ColsX[j] == LeaveX)
		_ColsX[j] = LeaveX->NextR;
	else{
		for (CurX=_ColsX[j]; CurX != NULL; CurX = CurX->NextR){
			if (CurX->NextR == LeaveX){
				CurX->NextR = CurX->NextR->NextR;
				break;
			}
		}
	}
	
	/* SET _EnterX TO BE THE NEW EMPTY SLOT */
	_EnterX = LeaveX;
}

int CEmdWrapper::findLoop(node2_t **Loop){
	int i, steps;
	node2_t **CurX, *NewX;
	char IsUsed[2*MAX_SIG_SIZE1]; 
	
	for (i=0; i < _n1+_n2; i++)
		IsUsed[i] = 0;
	
	CurX = Loop;
	NewX = *CurX = _EnterX;
	IsUsed[_EnterX-_X] = 1;
	steps = 1;
	
	do
    {
		if (steps%2 == 1){
			/* FIND AN UNUSED X IN THE ROW */
			NewX = _RowsX[NewX->i];
			while (NewX != NULL && IsUsed[NewX-_X])
				NewX = NewX->NextC;
		}else{
			/* FIND AN UNUSED X IN THE COLUMN, OR THE ENTERING X */
			NewX = _ColsX[NewX->j];
			while (NewX != NULL && IsUsed[NewX-_X] && NewX != _EnterX)
				NewX = NewX->NextR;
			if (NewX == _EnterX)
				break;
		}
		
		if (NewX != NULL){/* FOUND THE NEXT X */
			/* ADD X TO THE LOOP */
			*++CurX = NewX;
			IsUsed[NewX-_X] = 1;
			steps++;
		}else{/* DIDN'T FIND THE NEXT X */
			/* BACKTRACK */
			do
			{
				NewX = *CurX;
				do 
				{
					if (steps%2 == 1)
						NewX = NewX->NextR;
					else
						NewX = NewX->NextC;
				} while (NewX != NULL && IsUsed[NewX-_X]);
				
				if (NewX == NULL){
					IsUsed[*CurX-_X] = 0;
					CurX--;
					steps--;
				}
			} while (NewX == NULL && CurX >= Loop);
			
			IsUsed[*CurX-_X] = 0;
			*CurX = NewX;
			IsUsed[NewX-_X] = 1;
		}     
    } while(CurX >= Loop);
	
	if (CurX == Loop){
		fprintf(stderr, "emd: Unexpected error in findLoop!\n");
		exit(1);
    }
	
	return steps;
}

void CEmdWrapper::russel(float *S, float *D){
	int i, j, found, minI, minJ;
	float deltaMin, oldVal, diff;
	float Delta[MAX_SIG_SIZE1][MAX_SIG_SIZE1];
	node1_t Ur[MAX_SIG_SIZE1], Vr[MAX_SIG_SIZE1];
	node1_t uHead, *CurU, *PrevU;
	node1_t vHead, *CurV, *PrevV;
	node1_t *PrevUMinI, *PrevVMinJ, *Remember;
	
	/* INITIALIZE THE ROWS LIST (Ur), AND THE COLUMNS LIST (Vr) */
	/* 0. Init List, Ur, Vr */
	uHead.Next = CurU = Ur;
	for (i=0; i < _n1; i++){
		CurU->i = i;
		CurU->val = -EMD_INFINITY;
		CurU->Next = CurU+1;
		CurU++;
    }
	(--CurU)->Next = NULL;
	
	vHead.Next = CurV = Vr;
	for (j=0; j < _n2; j++){
		CurV->i = j;
		CurV->val = -EMD_INFINITY;
		CurV->Next = CurV+1;
		CurV++;
    }
	(--CurV)->Next = NULL;
	
	/* 1. FIND THE MAXIMUM ROW AND COLUMN VALUES (Ur[i] AND Vr[j]) */
	float v;
	for(i=0; i < _n1 ; i++){
		for(j=0; j < _n2 ; j++){
			v = _C[i][j];
			if (Ur[i].val <= v)
				Ur[i].val = v;
			if (Vr[j].val <= v)
				Vr[j].val = v;
		}
	}

	/* 2. COMPUTE THE Delta MATRIX */
	for(i=0; i < _n1 ; i++){
		for(j=0; j < _n2 ; j++)
			Delta[i][j] = _C[i][j] - Ur[i].val - Vr[j].val;
	}

	/* 2. FIND THE BASIC VARIABLES  */
	do{	
		/* FIND THE SMALLEST Delta[i][j] */
		found = 0; 
		deltaMin = EMD_INFINITY;      
		PrevU = &uHead;
		for (CurU=uHead.Next; CurU != NULL; CurU=CurU->Next){
			int i;
			i = CurU->i;
			PrevV = &vHead;
			for (CurV=vHead.Next; CurV != NULL; CurV=CurV->Next){
				int j;
				j = CurV->i;
				if (deltaMin > Delta[i][j]){
					deltaMin = Delta[i][j];
					minI = i;
					minJ = j;
					PrevUMinI = PrevU;
					PrevVMinJ = PrevV;
					found = 1;
				}
				PrevV = CurV;
			}
			PrevU = CurU;
		}
		
		if (! found)
			break;
		
		/* ADD X[minI][minJ] TO THE BASIS, AND ADJUST SUPPLIES AND COST */
		Remember = PrevUMinI->Next;
		addBasicVariable(minI, minJ, S, D, PrevUMinI, PrevVMinJ, &uHead);
		
		/* UPDATE THE NECESSARY Delta[][] */
		if (Remember == PrevUMinI->Next){/* LINE minI WAS DELETED */
			for (CurV=vHead.Next; CurV != NULL; CurV=CurV->Next){
				int j;
				j = CurV->i;
				if (CurV->val == _C[minI][j]){/* COLUMN j NEEDS UPDATING */
					/* FIND THE NEW MAXIMUM VALUE IN THE COLUMN */
					oldVal = CurV->val;
					CurV->val = -EMD_INFINITY;
					for (CurU=uHead.Next; CurU != NULL; CurU=CurU->Next){
						int i;
						i = CurU->i;
						if (CurV->val <= _C[i][j])
							CurV->val = _C[i][j];
					}
					
					/* IF NEEDED, ADJUST THE RELEVANT Delta[*][j] */
					diff = oldVal - CurV->val;
					if (fabs(diff) < EPSILON * _maxC)
						for (CurU=uHead.Next; CurU != NULL; CurU=CurU->Next)
							Delta[CurU->i][j] += diff;
				}
			}
		}else{/* COLUMN minJ WAS DELETED */
			for (CurU=uHead.Next; CurU != NULL; CurU=CurU->Next){
				int i;
				i = CurU->i;
				if (CurU->val == _C[i][minJ]){/* ROW i NEEDS UPDATING */
					/* FIND THE NEW MAXIMUM VALUE IN THE ROW */
					oldVal = CurU->val;
					CurU->val = -EMD_INFINITY;
					for (CurV=vHead.Next; CurV != NULL; CurV=CurV->Next){
						int j;
						j = CurV->i;
						if(CurU->val <= _C[i][j])
							CurU->val = _C[i][j];
					}
					
					/* If NEEDED, ADJUST THE RELEVANT Delta[i][*] */
					diff = oldVal - CurU->val;
					if (fabs(diff) < EPSILON * _maxC)
						for (CurV=vHead.Next; CurV != NULL; CurV=CurV->Next)
							Delta[i][CurV->i] += diff;
				}
			}
		}
    } while (uHead.Next != NULL || vHead.Next != NULL);
}



void CEmdWrapper::addBasicVariable(int minI, int minJ, float *S, float *D, 
								   node1_t *PrevUMinI, node1_t *PrevVMinJ,
								   node1_t *UHead){
	float T;
	
	if (fabs(S[minI]-D[minJ]) > EPSILON * _maxW)/* DEGENERATE CASE */{/* DEGENERATE CASE */
		if (S[minI] < D[minJ]){/* SUPPLY EXHAUSTED */
			T = S[minI];
			S[minI] = 0;
			D[minJ] -= T; 
		}else{  /* DEMAND EXHAUSTED */
			T = D[minJ];
			D[minJ] = 0; 
			S[minI] -= T; 
		}
	}else{
		T = S[minI];
		S[minI] = 0;
		D[minJ] -= T; 
	}
	
	/* X(minI,minJ) IS A BASIC VARIABLE */
	_IsX[minI][minJ] = 1; 
	
	_EndX->val = T;
	_EndX->i = minI;
	_EndX->j = minJ;
	_EndX->NextC = _RowsX[minI];
	_EndX->NextR = _ColsX[minJ];
	_RowsX[minI] = _EndX;
	_ColsX[minJ] = _EndX;
	_EndX++;
	
	/* DELETE SUPPLY ROW ONLY IF THE EMPTY, AND IF NOT LAST ROW */
	if (S[minI] == 0 && UHead->Next->Next != NULL)
		PrevUMinI->Next = PrevUMinI->Next->Next;  /* REMOVE ROW FROM LIST */
	else
		PrevVMinJ->Next = PrevVMinJ->Next->Next;  /* REMOVE COLUMN FROM LIST */
}

int CEmdWrapper::isOptimal(node1_t *U, node1_t *V){    
	float delta, deltaMin;
	int i, j, minI, minJ;
	
	/* FIND THE MINIMAL Cij-Ui-Vj OVER ALL i,j */
	
	float val;
	deltaMin = (float)EMD_INFINITY;
	for(i=0; i < _n1; i++){
		val=U[i].val;
		for(j=0; j < _n2; j++){
			if (! _IsX[i][j]){
				delta = _C[i][j] - val - V[j].val;
				if (deltaMin > delta){
					deltaMin = delta;
					minI = i;
					minJ = j;
				}
			}
		}
	}
	
	_EnterX->i = minI;
	_EnterX->j = minJ;
	
	/* IF NO NEGATIVE deltaMin, WE FOUND THE OPTIMAL SOLUTION */
	return deltaMin >= -EPSILON * _maxC;
}



