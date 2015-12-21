/*
* SDSP.h SDHumming Digital Signal Processing routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/

#include "SDSP.h"
#include <stdio.h>
#include <string.h>

double DotProduct (const double *dpSrc1, const double *dpSrc2,long lCount){
	long lLoopCntr;
	double dDotProd = 0.0;

	for (lLoopCntr = 0L; lLoopCntr < lCount; lLoopCntr++){
		dDotProd += dpSrc1[lLoopCntr] * dpSrc2[lLoopCntr];
	}
	return dDotProd;
}

float DotProduct (const float *dpSrc1, const float *dpSrc2,long lCount){
	long lLoopCntr;
	float dDotProd = 0.0;

	for (lLoopCntr = 0L; lLoopCntr < lCount; lLoopCntr++){
		dDotProd += dpSrc1[lLoopCntr] * dpSrc2[lLoopCntr];
	}
	return dDotProd;
}


void CplxMul (Complex* spCplxDest, double dSrc){
	spCplxDest->R *= dSrc;
	spCplxDest->I *= dSrc;
}

void Mul (Complex* spCplxDest, double dSrc, long lCount){
	long lLoopCntr;

	for (lLoopCntr = 0L; lLoopCntr < lCount; lLoopCntr++)	{
		CplxMul(&spCplxDest[lLoopCntr], dSrc);
	}
}

void CplxMul (Complex* spCplxDest, const Complex* spCplxSrc1,
			  const Complex* spCplxSrc2){
	spCplxDest->R =
		spCplxSrc1->R * spCplxSrc2->R - spCplxSrc1->I * spCplxSrc2->I;
	spCplxDest->I =
		spCplxSrc1->R * spCplxSrc2->I + spCplxSrc1->I * spCplxSrc2->R;
}


void Mul (Complex* spCplxDest, const Complex* spCplxSrc1,
		  const Complex* spCplxSrc2, long lCount){
	long lLoopCntr;

	for (lLoopCntr = 0L; lLoopCntr < lCount; lLoopCntr++){
		CplxMul(&spCplxDest[lLoopCntr], &spCplxSrc1[lLoopCntr],
			&spCplxSrc2[lLoopCntr]);
	}
}

Complex ProdC(Complex Src1, Complex Src2){
	Complex dest;
	Mul(&dest, &Src1, &Src2, 1);
	return dest;
}

Complex* ProdC(Complex* Src1, int nLen,  double Src2){
	long i;
	Complex* dst=new Complex[nLen];

	for (i=0; i < nLen; i++){
		dst[i].R=Src1[i].R*Src2;
		dst[i].I=Src1[i].I*Src2;
	}
	return dst;
}

void ProdC(Complex* Src1, int nLen,  double Src2, Complex* dst){
	long i;

	for (i=0; i < nLen; i++){
		dst[i].R=Src1[i].R*Src2;
		dst[i].I=Src1[i].I*Src2;
	}
	return ;
}

Complex* UnosC(long Src){
	long i;
	Complex* dest=new Complex[Src];

	for (i=0; i < Src; i++){
		dest[i].R = 1;
		dest[i].I = 0;
	}
	return dest;
}

void CplxDiv (Complex* spCplxDest, const Complex* spCplxSrc1,
			  const Complex* spCplxSrc2){
	spCplxDest->R =
		(spCplxSrc1->R * spCplxSrc2->R + spCplxSrc1->I * spCplxSrc2->I) /
		(spCplxSrc2->R * spCplxSrc2->R + spCplxSrc2->I * spCplxSrc2->I);
	spCplxDest->I =
		(spCplxSrc1->I * spCplxSrc2->R - spCplxSrc1->R * spCplxSrc2->I) /
		(spCplxSrc2->R * spCplxSrc2->R + spCplxSrc2->I * spCplxSrc2->I);
}


void Div (Complex* spCplxDest, const Complex* spCplxSrc1,
		  const Complex* spCplxSrc2, long lCount){
	long lLoopCntr;

	for (lLoopCntr = 0L; lLoopCntr < lCount; lLoopCntr++)	{
		CplxDiv(&spCplxDest[lLoopCntr], &spCplxSrc1[lLoopCntr],
			&spCplxSrc2[lLoopCntr]);
	}
}

Complex DivC(Complex Src1, Complex Src2){
	Complex dest;
	Div(&dest, &Src1, &Src2, 1);

	return dest;
}

Complex DivC(double Src1, Complex Src2){
	Complex dest, temp;
	temp.R = Src1;
	temp.I = 0;
	dest = DivC(temp, Src2);

	return dest;
}

Complex ProdC(Complex Src1, double Src2){
	Complex dest = Src1;
	Mul(&dest, Src2, 1);

	return dest;
}

void CplxDiv (Complex* spCplxDest, const Complex* spCplxSrc){
	double dReal;
	double dImag;

	dReal = (spCplxDest->R * spCplxSrc->R + spCplxDest->I * spCplxSrc->I) /
		(spCplxSrc->R * spCplxSrc->R + spCplxSrc->I * spCplxSrc->I);
	dImag = (spCplxDest->I * spCplxSrc->R - spCplxDest->R * spCplxSrc->I) /
		(spCplxSrc->R * spCplxSrc->R + spCplxSrc->I * spCplxSrc->I);
	spCplxDest->R = dReal;
	spCplxDest->I = dImag;
}

void Div (Complex* spCplxDest, const Complex* spCplxSrc, long lCount){
	long lLoopCntr;

	for (lLoopCntr = 0L; lLoopCntr < lCount; lLoopCntr++){
		CplxDiv(&spCplxDest[lLoopCntr], &spCplxSrc[lLoopCntr]);
	}
}

Complex* Sqr(Complex* Src,int nLen){
	long N = nLen;
	Complex* dest=new Complex[N];

	for (long i=0; i < N; i++){
		dest[i].R = pow(Src[i].R,2) - pow(Src[i].I,2);
		dest[i].I = 2 * Src[i].R * Src[i].I;
	}
	return dest;
}

void Sqr(Complex* Src,int nLen, Complex* dst){
	long N = nLen;

	for (long i=0; i < N; i++){
		dst[i].R = pow(Src[i].R,2) - pow(Src[i].I,2);
		dst[i].I = 2 * Src[i].R * Src[i].I;
	}
	return ;
}

void Cart2Polar (SPolar* spPolar, const Complex* spCplx){
	spPolar->M = sqrt(spCplx->R * spCplx->R + spCplx->I * spCplx->I);
	spPolar->P = atan2(spCplx->I, spCplx->R);
}


void CartToPolar (SPolar* spPolar, const Complex* spCart,
				  long lCount){
	long lLoopCntr;

	for (lLoopCntr = 0L; lLoopCntr < lCount; lLoopCntr++){
		Cart2Polar(&spPolar[lLoopCntr], &spCart[lLoopCntr]);
	}
}

void Polar2Cart (Complex* spCplx, const SPolar* spPolar){
	spCplx->R = spPolar->M * cos(spPolar->P);
	spCplx->I = spPolar->M * sin(spPolar->P);
}


void PolarToCart (Complex* spCart, const SPolar* spPolar,
				  long lCount){
	long lLoopCntr;

	for (lLoopCntr = 0L; lLoopCntr < lCount; lLoopCntr++){
		Polar2Cart(&spCart[lLoopCntr], &spPolar[lLoopCntr]);
	}
}

Complex* Sqrt(Complex* Src,int nLen){

	long N = nLen;
	SPolar* auxPolar=new SPolar[N];
	Complex* dest=new Complex[N];

	CartToPolar(auxPolar,Src,N);

	for (long i=0; i < N; i++){
		auxPolar[i].M = sqrt(auxPolar[i].M);
		auxPolar[i].P /= 2;
	}

	PolarToCart(dest,auxPolar,N);
	delete[] auxPolar;
	return dest;
}

void Sqrt(Complex* Src,int nLen, Complex* dest){

	long N = nLen;
	SPolar* auxPolar=new SPolar[N];

	CartToPolar(auxPolar,Src,N);

	for (long i=0; i < N; i++){
		auxPolar[i].M = sqrt(auxPolar[i].M);
		auxPolar[i].P /= 2;
	}

	PolarToCart(dest,auxPolar,N);
	delete[] auxPolar;
	return;
}


Complex RestaC(Complex Src1, Complex Src2){
	Complex dest;

	dest = Src1;
	dest.R-=Src2.R;
	dest.I-=Src2.I;

	return dest;
}

double* poly(Complex* x, int N){
	int i=0,j=0;
	Complex* aux=new Complex[N + 1];
	Complex* temp=new Complex[N+1];
	double* coefs=new double[N + 1];
	
	for ( i = 0; i < N+1; i++){
		aux[i].R=0;
		aux[i].I=0;
	}

	aux[0].R = 1;
	for ( i = 0; i < N; i++){
		for ( j = 0; j < N+1; j++){
			temp[j]= aux[j];
		}
		for ( j = 1; j < i+2; j++){
			aux[j] = RestaC(temp[j], ProdC(x[i],temp[j-1]));
		}
	}

	for ( i = 0; i < N+1; i++)
		coefs[i] = aux[i].R;

	delete[] aux;
	delete[] temp;
	return coefs;
}

float * LeastSquare(float A[3][5], int nLen, float *fData, int N){
	int i, j, k;
	float Q[3][5];
	float R[3][5];
	float QTb[3];
	int nPoints = 5;

	for( j=0;j<nPoints;j++){
		Q[0][j] = A[0][j];
		Q[1][j] = A[1][j];
		Q[2][j] = A[2][j];
	}

	for( j=0;j<N;j++){
		R[0][j] = 0;
		R[1][j] = 0;
		R[2][j] = 0;
		QTb[j]=0;
	}
	float temp;

	float *xv=new float[N];
	float tempVec1[5];
	for (k = 0; k < N; k++){		
		float fSum=0.0f;
		int x;
		for( x=0;x<nPoints;x++){
			fSum+=(Q[k][x]*Q[k][x]);
		}

		R[k][k] = (float)sqrt(fSum);
		for( x=0;x<nPoints;x++){
			Q[k][x] /= R[k][k];
		}

		for(j = k+1; j < N; j++){
			R[k][j] = DotProduct(Q[k],Q[j],nPoints);		
			for( x=0;x<nPoints;x++){
				tempVec1[x] =Q[k][x] * R[k][j];
			}	

			for( x=0;x<nPoints;x++){
				Q[j][x] -= tempVec1[x];
			}			
		}
	}

	for(j=0; j<N; j++)
		for(k=0; k<nPoints; k++)
			QTb[j] += (Q[j][k]*fData[k]);

	xv[N-1] = QTb[N-1]/R[N-1][N-1];
	for (i = N-2; i>=0; i--){
		temp = 0;
		for (k = i+1; k < N; k++)
			temp += R[i][k]*xv[k];
		xv[i] = (QTb[i]-temp)/R[i][i];
	}
	return xv;
}

void ParabolicInterpolation(float &Min, float &Pos, long i,float *m_datos){
	const int nPoints = 5;
	const int orden = 2;
	int j;

	float A[3][5];

	float *x;
	float km, di;

	for (j=0; j<nPoints; j++){
		A[1][j]=(float)(i+j-2);
	}

	for (j=0; j<nPoints; j++){
		A[2][j]=A[1][j];
	}
	for (j=0; j<nPoints; j++){
		A[2][j]*=A[2][j];
		A[0][j]=1;
	}

	x = LeastSquare(A,nPoints, m_datos+i-2, orden+1);
	km = -x[1]/(2*x[2]);
	di = (float)(x[2]*pow(km,2)+x[1]*km+x[0]);

	Min = di;
	Pos = km;

	delete[] x;
}

float SCalcPitchValue(float *fData, int nDataLen, float m_pendiente,int nIndexMax, int nIndexMin, int fs){
	const float VoiceThreshold = 0.17f;
	float min, minimum, posMin;
	long indMin;
	float fThreshold, f0;

	min=9999999.9f;
	for(int x=0;x<nDataLen;x++){
		if(min>fData[x]){
			min=fData[x];
			indMin=x;
		}
	}

	fThreshold = VoiceThreshold + m_pendiente*(indMin+1);

	if ((min > fThreshold) || (indMin > (nIndexMax-3))){
		f0 = 0;
	}else{
		float minValue=99999.0f;
		float minPos=-1.0f;
		for (long i=nIndexMin; i < nIndexMax-2; i++){
			if ((fData[i-1] > fData[i]) & (fData[i+1] > fData[i]) & (fData[i] < VoiceThreshold + m_pendiente*(i+1))){
				ParabolicInterpolation(minimum, posMin, i,fData);
				if(minValue>minimum){
					minValue=minimum;
					minPos=posMin;
				}
			}
		}
		if(minPos<0)
			f0=0.0;
		else
			f0=fs/minPos;
	}
	return f0;
}

Complex ComputeZeros(Complex* &nzr, Complex* zc, int ns, double* numtbil, double* dentbil,Complex* a,Complex* b,Complex* c){
	int i=0;
	Complex* tmp=new Complex[ns];
	Complex* aux1 = new Complex[ns];
	Complex* aux2 = new Complex[ns];
	Complex* aux3 = new Complex[ns];
	Complex* aux4 = new Complex[ns];
	Complex* aux5 = new Complex[ns];
	Complex* aux6 = new Complex[ns];
	Complex* vect1 = new Complex[ns]; 
	Complex* vect2 = new Complex[ns];
	Complex* vect3 = new Complex[ns];
	Complex* vect4 = new Complex[ns];


	for ( i=0; i < ns;i++){
		tmp[i].R = 1;
		tmp[i].I = 0;
	}
	ProdC(tmp,ns, numtbil[2],aux1);   
	ProdC(zc,ns, dentbil[2],aux2);    
	ProdC(tmp,ns, numtbil[1],aux3);   
	ProdC(zc,ns, dentbil[1],aux4);    
	ProdC(tmp,ns, numtbil[0],aux5);   
	ProdC(zc, ns, dentbil[0],aux6);   

	 for(i=0;i<ns;i++){
		 a[i].R=aux1[i].R - aux2[i].R;
		 a[i].I=aux1[i].I - aux2[i].I;
		 b[i].R=aux3[i].R - aux4[i].R;
		 b[i].I=aux3[i].I - aux4[i].I;
		 c[i].R=aux5[i].R - aux6[i].R;
		 c[i].I=aux5[i].I - aux6[i].I;
	 }

	 Complex cProd;
	 cProd.R=1;
	 cProd.I=0;
	 for(i=0;i<ns;i++){
		 cProd=ProdC(a[i], cProd);
	 }
	Complex  c1=cProd;

	 ProdC(a,ns, 2,aux1);

	 Div(c, a,ns);
	 memcpy(aux2, c,sizeof(Complex)*ns);
	 Div(b, aux1,ns);
	 memcpy(vect3, b,sizeof(Complex)*ns);

	 Sqr(vect3,ns,aux3);
	 for( i=0;i<ns;i++){
		 vect4[i].R=aux3[i].R - aux2[i].R;
		 vect4[i].I=aux3[i].I - aux2[i].I;
	 }
	 Sqrt(vect4,ns,aux4);

	 for( i=0;i<ns;i++){
		 vect1[i].R=aux4[i].R - vect3[i].R;
		 vect1[i].I=aux4[i].I - vect3[i].I;
	 }

	 for( i=0;i<ns;i++){
		 tmp[i].R=aux4[i].R + vect3[i].R;
		 tmp[i].I=aux4[i].I + vect3[i].I;
	 }
	 
	 ProdC(tmp,ns, -1,vect2); 
	 memcpy(nzr, vect1,sizeof(Complex)*ns);
	 for ( i = 0; i < ns; i++)
		 nzr[ns+i]=vect2[i];

	 delete[] aux1;
	 delete[] aux2;
	 delete[] aux3;
	 delete[] aux4;
	 delete[] aux5;
	 delete[] aux6;
	 delete[] vect1;
	 delete[] vect2;
	 delete[] vect3;
	 delete[] vect4;
	 delete[] tmp;
	 return c1;
}

Complex ComputePolar(Complex* &npl, Complex* pc,int ms, double* numtbil, double* dentbil,Complex* a,Complex* b,Complex* c){
	int i=0;
	Complex* tmp=new Complex[ms];
	Complex* aux1 = new Complex[ms];
	Complex* aux2 = new Complex[ms];
	Complex* aux3 = new Complex[ms];
	Complex* aux4 = new Complex[ms];
	Complex* aux5 = new Complex[ms];
	Complex* aux6 = new Complex[ms];
	Complex* vect1 = new Complex[ms];
	Complex* vect2 = new Complex[ms];
	Complex* vect3 = new Complex[ms];
	Complex* vect4 = new Complex[ms];

	for ( i=0; i < ms;i++){
		tmp[i].R = 1;
		tmp[i].I = 0;
	}

	ProdC(tmp, ms, numtbil[2],aux1);  
	ProdC(pc,ms, dentbil[2],aux2);
	ProdC(tmp,ms, numtbil[1],aux3);
	ProdC(pc,ms, dentbil[1],aux4);
	ProdC(tmp,ms, numtbil[0],aux5);
	ProdC(pc,ms, dentbil[0],aux6);

	for(i=0;i<ms;i++){
		 a[i].R=aux1[i].R - aux2[i].R;
		 a[i].I=aux1[i].I - aux2[i].I;
		 b[i].R=aux3[i].R - aux4[i].R;
		 b[i].I=aux3[i].I - aux4[i].I;
		 c[i].R=aux5[i].R - aux6[i].R;
		 c[i].I=aux5[i].I - aux6[i].I;
	 }
	 Complex cProd;
	 cProd.R=1;
	 cProd.I=0;
	 for( i=0;i<ms;i++){
		 cProd=ProdC(a[i], cProd);
	 }
	 Complex c2=cProd;

	 ProdC(a,ms,2,aux1);
	 Div(c, a,ms);
	  memcpy(aux2, c,sizeof(Complex)*ms);

	 Div(b, aux1,ms);
	  memcpy(vect3, b,sizeof(Complex)*ms);
	  
	 Sqr(vect3,ms,aux3);
	 for( i=0;i<ms;i++){
		 vect4[i].R=aux3[i].R - aux2[i].R;
		 vect4[i].I=aux3[i].I - aux2[i].I;
	 }
	 
	 Sqrt(vect4,ms,aux4);

	 for( i=0;i<ms;i++){
		 vect1[i].R=aux4[i].R - vect3[i].R;
		 vect1[i].I=aux4[i].I - vect3[i].I;
	 }

	 for(i=0;i<ms;i++){
		 tmp[i].R=aux4[i].R + vect3[i].R;
		 tmp[i].I=aux4[i].I + vect3[i].I;
	 }
	 ProdC(tmp,ms, -1,vect2);
	 memcpy(npl, vect1,sizeof(Complex)*ms);

	 for ( i = 0; i < ms; i++)
		 npl[i+ms]=vect2[i];

	 delete[] aux1;
	 delete[] aux2;
	 delete[] aux3;
	 delete[] aux4;
	 delete[] aux5;
	 delete[] aux6;
	 delete[] vect1;
	 delete[] vect2;
	 delete[] vect3;
	 delete[] vect4;
     delete[] tmp;

	 return c2;
}

Complex BilFunc1(Complex* pc,int ms,double* numtbil, double* dentbil, Complex *vect1){
	Complex c1;
	c1.R=1;
	c1.I=0;
	if(ms>0){
		Complex *aux1 = new Complex[ms];
		Complex *aux2 = new Complex[ms];
		ProdC(pc,ms, dentbil[1], aux1);
		Complex *tmp1 = UnosC(ms);
		ProdC(tmp1,ms, numtbil[1],aux2);
		int j=0;
		for( j=0;j<ms;j++){
		 vect1[j].R=aux2[j].R - aux1[j].R;
		 vect1[j].I=aux2[j].I - aux1[j].I;
		}
		Complex cProd;
		cProd.R=1;
		cProd.I=0;
		for( j=0;j<ms;j++){
		 cProd=ProdC(vect1[j], cProd);
		}
		c1=cProd;
		delete[] tmp1;
		delete[] aux1;
		delete[] aux2;
	}
	 return c1;
}

void BilFunc2(Complex* pc,int ms,double* numtbil, double* dentbil, Complex *vect1){
	Complex *aux1 = new Complex[ms];
	Complex *aux2 = new Complex[ms];

	ProdC(pc,ms, dentbil[0],aux1);
	Complex *tmp = UnosC(ms);
	ProdC(tmp,ms, numtbil[0],aux2);

	 for(int j=0;j<ms;j++){
		 vect1[j].R=aux1[j].R - aux2[j].R;
		 vect1[j].I=aux1[j].I - aux2[j].I;
	 }

	 delete[] aux1;
	 delete[] aux2;
	 delete[] tmp;
}

int Getbilinear(Complex* &npl, Complex* &nzr, double &ngn, 
		 Complex* pc,int ms, Complex* zc, int ns, double gc, 
		 double* numtbil, double* dentbil){

	Complex *vect1=NULL;
	Complex *vect2=NULL;
	Complex* vect3=NULL;
	Complex* vect4=NULL;
 
	if(ms>0)
		vect1=new Complex[ms];
    Complex c1 = BilFunc1(pc, ms, numtbil, dentbil,vect1);
 
	if(ns>0)
		vect2=new Complex[ns];
	Complex c2 = BilFunc1(zc, ns, numtbil, dentbil,vect2);
	
 	 Complex ngnaux;
	 if (ns == 0){
		 ngnaux = DivC(1.f, c1);
	 }else{
		 ngnaux = DivC(c2, c1);
	 }

	 Complex c3; 
	 if (ms != ns){
		 c3 = ProdC(ngnaux, gc * pow(dentbil[1], ms - ns));
	 }else{
		 c3 = ProdC(ngnaux, gc);
	 }

	 ngn = c3.R;
	 if (ns != 0){
		 vect3=new Complex[ns];
		 BilFunc2(zc,ns, numtbil, dentbil, vect3);
	 }
	
	 vect4=new Complex[ms];
	 BilFunc2(pc,ms, numtbil, dentbil, vect4);

	 if(ns!=0){
		 Div(vect3, vect2,ns);
		 memcpy(nzr, vect3, sizeof(Complex)*ns);
	 }
	 Div(vect4, vect1,ms);
	 memcpy(npl, vect4, sizeof(Complex)*ms);

	 if(NULL!=vect1){
		delete[] vect1;
		vect1=NULL;
	 }
	 if(NULL!=vect2){
		delete[] vect2;
		vect2=NULL;
	 }
	 if(NULL!=vect3){ 
		delete[] vect3;
		vect3=NULL;
	 }
	 if(NULL!=vect4){
		delete[] vect4;
		vect4=NULL;
	 }

	 c1.R = -dentbil[0] / dentbil[1];
	 c1.I = 0;
	 if (ms > ns){
		 for (int i = 0; i < ms - ns; i++)
			 nzr[i+ns]=c1;
	 }
	return 0;
}

float ComputeGain(double gc, Complex c1, Complex c2, double dentbil2,int ms, int ns){
	float RetVal=0.0f;
	Complex c3 = DivC(c1,c2);
	Complex ngnaux = ProdC(c3, gc);
	c3 = ProdC(ngnaux, pow(dentbil2, ms - ns)); 
	RetVal = (float) c3.R;
	return RetVal;
}


//https://ccrma.stanford.edu/~jos/pasp/Bilinear_Transformation.html
//http://blog.chinaaet.com/detail/3431.html
int bilt(Complex* &npl, Complex* &nzr, double &ngn, 
		 Complex* pc,int ms, Complex* zc, int ns, double gc, 
		 double* numtbil, double* dentbil,int bilSize){

	 if (bilSize == 2){
		Getbilinear(npl, nzr, ngn, pc,ms, zc, ns, gc, numtbil, dentbil);
		return ms;
	 }else if (bilSize == 3){
		 Complex c1, c2;
		 delete[] npl;
		 delete[] nzr;
		 npl=new Complex[ms*2];
		 nzr=new Complex[ns*2];

		 Complex *a, *b, *c;
		 a=new Complex[ns];
		 b=new Complex[ns];
		 c=new Complex[ns];

		 c1 = ComputeZeros(nzr, zc, ns, numtbil, dentbil,a,b,c);
		 c2 = ComputePolar(npl, pc, ms, numtbil, dentbil, a, b, c);
		 ngn = ComputeGain(gc, c1, c2, dentbil[2],ms, ns);

		 delete[] a;
		 delete[] b;
		 delete[] c;
		 return ms*2;
	 }else{
		 printf("Wrong parameter in Bilinear transform\n");
		 return 0;
	 }
}

void Polar2Cart (Complex &spCplx, const SPolar spPolar){
	spCplx.R = spPolar.M * cos(spPolar.P);
	spCplx.I = spPolar.M * sin(spPolar.P);
}

void ComputePolarGain(Complex* pc, double &gc, int n, float omegac){
	const double pi = acos(-1.f);
	const double fasei = pi/2 + pi/(2*n);
	SPolar *pcPolar=new SPolar[n];
	int i=0;
	for (i = 0; i < n; i++){
		pcPolar[i].M = omegac;
		pcPolar[i].P = fasei + i*pi/n;
	}

	for (i = 0; i < n; i++){
		Polar2Cart(pc[i], pcPolar[i]);
	}

	gc = fabs(pow(-omegac,n));
	delete[] pcPolar;
}


void trans(Complex* pd, int pdNum, Complex* zd,int zdNum,double gd, int tipo, float* fcorte, double* &m_acoef,double* &m_bcoef){

	float fc, f0, f1;
	double alfa, k, gdc;
	const float pi = (float)acos(-1.f);
	double* num, *den;
	Complex* pdc, *zdc;

	fc = 0.25;
	f0 = fcorte[0];
	f1 = fcorte[1];

	int nDim=0;
	switch (tipo){
	case 0:
		alfa = sinf(pi * (fc - f0)) / sinf(pi * (fc + f0));
		num=new double[2];
		den=new double[2];
		nDim=2;
		num[0] = -alfa;
		num[1] = 1;
		den[0] = 1;
		den[1] = -alfa;
		break;
	case 1:
		alfa = -cosf(pi * (fc - f0)) / cosf(pi * (fc + f0));
		num=new double[2];
		den=new double[2];
		nDim=2;
		num[0] = -1;
		num[1] = -alfa;
		den[0] = alfa;
		den[1] = 1;
		break;
	case 2:
		k = tanf(pi * fc) / tanf(pi * (f0 - f1) );
		alfa = cosf(pi * (f0 + f1)) / cosf(pi * (f0 - f1));
		num=new double[3];
		den=new double[3];
		nDim=3;
		num[0] = -(k + 1);
		num[1] = 2 * alfa * k;
		num[2] = -(k - 1);
		den[0] = -num[2] ;
		den[1] = -num[1];
		den[2] = -num[0];
		break;
	default:
		printf("Tipo de filtro incorrecto.\n");
		break;
	}
	pdc=new Complex[pdNum];
	zdc=new Complex[zdNum];
	int orden=bilt(pdc, zdc, gdc, pd,pdNum, zd,zdNum, gd, num, den,nDim);
	
	m_acoef = poly(zdc,orden);
	for(int i=0;i<orden+1;i++)
		m_acoef[i]*=gdc;
	m_bcoef = poly(pdc,orden);

	delete[] num;
	delete[] den;
	delete[] pdc;
	delete[] zdc;
}

void SProSignal(int orden, int tipo, float* fcorte, double* &m_acoef,double* &m_bcoef){
	Complex* pc=new Complex[orden];
	Complex* zc=new Complex[orden];
	double gc;
	float omega = 2;  

	Complex* npl=new Complex[orden];
	Complex* nzr=new Complex[orden];
	double ngn;

	double numtbil[2];
	double dentbil[2];

	ComputePolarGain(pc, gc, orden, omega);

	numtbil[0] = -2;
	numtbil[1] = 2;
	dentbil[0] =1;
	dentbil[1] =1;

	bilt(npl, nzr, ngn, pc,orden, zc, 0, gc, numtbil, dentbil,2);
	trans(npl, orden, nzr,orden, ngn, tipo, fcorte, m_acoef, m_bcoef);
	delete[] pc;
	delete[] zc; 
	delete[] npl;
	delete[] nzr;
}

void Reverse(double* dpSrc,int N){
	double *tmp=new double[N];
	memcpy(tmp,dpSrc,sizeof(double)*N);

	long lLoopCntr;
	long lMax;

	lMax = N - 1L;
	for (lLoopCntr = 0L; lLoopCntr <= lMax; lLoopCntr++){
		dpSrc[lLoopCntr] = tmp[lMax - lLoopCntr];
	}
	delete[] tmp;
}

void SFilter(float *fData,int nDataLen,double* m_acoef,double *m_bcoef,int N){
	double *y=new double[nDataLen];
	double *b=new double[N-1];
	double *x=new double[N];
	double *yp=new double[N-1];
	memset(y,0,nDataLen*sizeof(double));
	memcpy(b, m_bcoef+1, (N-1)*sizeof(double));;

	long i=0;
	int j=0;
	for ( i = 0; i < N-1; i++){
		long k=0;
		for ( k = 0; k <= i; k++)
			y[i] += m_acoef[k] * fData[i - k];

		for (k = 1; k <= i; k++)
			y[i] -= m_bcoef[k] * y[i - k];

	}

	for (i = N - 1; i < nDataLen; i++ ){
		for( j=0;j<N;j++){
			x[j]=fData[i - (N - 1)+j];
		}

		for( j=0;j<N-1;j++){
			yp[j] = y[i - (N - 1)+j];	
		}

		Reverse(x,N);
		Reverse(yp,N-1);

		double a1=DotProduct(m_acoef, x,N);
		double a2=DotProduct(b, yp,N-1);

		y[i] = a1 - a2;
	}

	for(i=0;i<nDataLen;i++){
		fData[i] = (float)y[i];
	}

	delete[] y;
	delete[] b;
	delete[] x;
	delete[] yp;

}


void SCalcDifFunction(const float *fData,int nFrm, int nIndexMax, float fPend,float *oDataBuf){
	float	sumd = 0;
	float dif;
	oDataBuf[0] = 1 + fPend;
	for ( int i = 1; i <= nIndexMax; i++ ){
		dif=0.0f;
		for(int j=0;j<nFrm+1;j++){
			dif+=(fData[j]-fData[i+j])*(fData[j]-fData[i+j]);
		}
		sumd += dif;
		oDataBuf[i] = dif/(sumd/i) + (i+1) * fPend;
	}
}


float GetMean(const float *fData, int nLen){
	float fSum=0.0f;
	for(int i=0;i<nLen;i++){
		fSum+=fData[i];
	}
	return fSum/nLen;
}


void GetEnergyCurve (float *fpDest, const float *fpSrc, long nFrmSht, long nSamplePoints){
	long i;
	long nFrms;

	nFrms = nSamplePoints / nFrmSht;
	for (i = 0; i < nFrms; i++)
		fpDest[i] = GetMean(fpSrc+i*nFrmSht,nFrmSht);
}

float SGetEnergyVec(float *fData, int nLen, long nFrmSht, float *&EnergyVec, int &nEnergyLen){
	float * fDataTemp, *fcorte;
	fDataTemp=new float[nLen];
	memcpy(fDataTemp,fData,sizeof(float)*nLen);
 
	for(int i=0;i<nLen;i++){
		fDataTemp[i]=fabsf(fDataTemp[i]);
	}

	fcorte=new float[2];
	fcorte[0] =(float) (0.8 / (2 * nFrmSht));
	double* m_acoef=NULL;
	double* m_bcoef=NULL;
	SProSignal(6, 0, fcorte,m_acoef,m_bcoef); 
	SFilter(fDataTemp, nLen,m_acoef,m_bcoef,7);


	int lMax=(nLen-nFrmSht)/nFrmSht;
	nEnergyLen=lMax;
	EnergyVec=new float[lMax];
	GetEnergyCurve(EnergyVec, fDataTemp+nFrmSht, nFrmSht,nLen-nFrmSht);

	float fMeanEnergyValue=GetMean(EnergyVec, nEnergyLen);

	delete[] m_acoef;
	delete[] m_bcoef;
	delete[] fDataTemp;
	delete[] fcorte;
	return fMeanEnergyValue;
}
