////////////////////////////////////////////////////////////////////////////////
/**
 * @file   SNoteMatch.h
 * @author Wang Lei <wanglei.will@snda.com>
 * @date   Thu Aug 19 10:37:14 2010
 * 
 * @brief  
 * 
 * 
 */
////////////////////////////////////////////////////////////////////////////////
/*
* For measuring the similarity between two note sequence
*
* Suppose one point set as "supplier" which is a number of hillocks, with
* which weight represented as the volume occupied. Another set as "demander"
* can be imaged as a mass of holes with a certain amount of capacities. EMD 
* measures the minimum flow work needed to transport earth of these hillocks 
* to fill the total amount of holes, with which the cost considering both 
* capacities and two dimensional positions. Such kinds of problem can be 
* transformed into the following linear programming. EMD has robust properties 
* to errors brought about in the front end. Note segmentation mistakes to 
* fragment note into two separate parts. In string matching, this action will 
* induce an insertion cost and otherwise a deletion cost from query to template. 
* But in EMD, Such error is trivial because EMD is in a top-down scale of view, 
* the contribution of query notes is approximate equal to the notes of template, 
* no matter how much fragmented or consolidated errors segmentation brings. As for 
* insertion or deletion error, the influence of string matching is two units cost, 
* but EMD is little, either. Moreover, a small altered weight or position of one note, 
* taken as replacement error, also cause an infinitesimal change in the global EMD 
* measure. Many note-based matching algorithms used in QBH systems are lack of using 
* both pitch and tempo information. Except that, DP based matching works in a bottom-up
* fashion. However, Long time information such as tempo and duration exist in a global
* view and are difficult to capture by bottom-up methods. But EMD measure works with
* optimization of both global pitch and tempo information in implicitly.
 */

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

#ifndef SDHumming_SNOTEMATCH_H_
#define SDHumming_SNOTEMATCH_H_

#include "SSearchCommon.h"
#include "STypes.h"

/*****************************************************************************/
/* feature_t SHOULD BE MODIFIED BY THE USER TO REFLECT THE FEATURE TYPE      */
typedef struct  {
	float px;
	float py;
} feature_t;
/*****************************************************************************/

typedef struct{
	int n;                // Number of features in the signature 
	feature_t *Features;  // Pointer to the features vector
	float *Weights;       // Pointer to the weights of the features
} signature_t;

typedef struct{
	int from;             // Feature number in signature 1 
	int to;               // Feature number in signature 2 
	float amount;         // Amount of flow from "from" to "to" 
} flow_t;

#define MAX_SIG_SIZE1  (MAX_SIG_SIZE)  /* FOR THE POSIBLE DUMMY FEATURE */

/* NEW TYPES DEFINITION */

/* node1_t */
typedef struct node1_t {
	int i;
	float val;
	struct node1_t *Next;
} node1_t;

/* node2_t */
typedef struct node2_t {
	int i, j;
	float val;
	struct node2_t *NextC;               
	struct node2_t *NextR;              
} node2_t;

float SModelNoteMatching(int FirstNum,SNote *Notes, int NoteSize, feature_t *m_FeatureQuery, float *m_WeightQuery, int m_LenQuery, int m_DurQuery);

float SQueryNoteProcessing(int FirstNum, SNote *Notes, int NoteSize,feature_t *m_FeatureQuery, float *m_WeightQuery, int &m_LenQuery, int &m_DurQuery);

typedef struct {
	int _n1, _n2, _Width;    
	node2_t _X[MAX_SIG_SIZE1*2];           
	float _C[MAX_SIG_SIZE1][MAX_SIG_SIZE1];  
	node2_t *_EndX, *_EnterX;
	char _IsX[MAX_SIG_SIZE1][MAX_SIG_SIZE1];
	node2_t *_RowsX[MAX_SIG_SIZE1], *_ColsX[MAX_SIG_SIZE1];
	double _maxW;
	float _maxC;

	float emd(signature_t *Signature1, signature_t *Signature2,flow_t *Flow, int *FlowSize);
	
	float init(signature_t *Signature1, signature_t *Signature2);
	void findBasicVariables(node1_t *U, node1_t *V);
	int isOptimal(node1_t *U, node1_t *V);
	int findLoop(node2_t **Loop);
	void newSol();
	void russel(float *S, float *D);
	void addBasicVariable(int minI, int minJ, float *S, float *D, 
							 node1_t *PrevUMinI, node1_t *PrevVMinJ,
							 node1_t *UHead);	

}CEmdWrapper;


#endif //SDHumming_SNOTEMATCH_H_
