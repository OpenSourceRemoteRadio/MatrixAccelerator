/*
Copyright (c) 2017, BigCat Wireless Pvt Ltd
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the copyright holder nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.



THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
 /**
* @file accelerator_main.c
* @brief This file defines all the API associated with working of Matrix vector row accelerator to calculate [B^T X A] for two matrices of size [7k x 13]
* @defgroup Matrix_accelerator
*/

/*******************************************************************************
* Include public/global header files
*******************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>

/*******************************************************************************
* Include user-defined header files
*******************************************************************************/
//#include "alt_types.h"
#include "mat_acc.h"
#include "mat_acc_firmware.h"
//#include "mat_acc_mm2st.h"
//#include "mat_acc_avst_to_avmm.h"
//#include "mat_acc_msgdma.h"

/*******************************************************************************
* Global variables
*******************************************************************************/
#define DUAL_ACC_INPUT
int fh;
char device_node[256];
acc_comp *A, *B, *C;
acc_comp *Crow, *Ccol;
acc_comp *ACCrow, *ACCcol;

int DebugLevel=DEBUG_LEVEL_ERROR;
volatile unsigned int * pb_msgdma_1_desc;
volatile unsigned int * pb_msgdma_2_desc;
volatile unsigned int * ddr_memmaped;
volatile unsigned int *pPattern_ctrl1, *pPattern_ctrl2, *pCapture_ctrl,*ACCEL_STATUS_ptr;
volatile unsigned int * CAPBRIDGE_ADDR_ptr;

#ifdef USER_DEFINED_DATA
float A[10001], B[10001], C[10001], D[10001];
#endif



/**
 * @fn void remaining_element_comp(acc_comp * __restrict__ A,acc_comp * __restrict__ B,acc_comp * __restrict__ Crow, acc_comp * __restrict__ Ccol,acc_comp * __restrict__ C)
 * @ingroup Matrix_accelerator
 * @brief This function computes remaining 12x12 elements in ARM given the input matrices, first row and column elements
 * @param A	INPUT MATRIX A of datatype acc_comp
 * @param B INPUT MATRIX B of datatype acc_comp
 * @param Crow First row elements of datatype acc_comp
 * @param Ccol First column elements of datatype acc_comp
 * @param C Output complete matrix of datatype acc_comp
 */
void remaining_element_comp(acc_comp * __restrict__ A,acc_comp * __restrict__ B,acc_comp * __restrict__ Crow, acc_comp * __restrict__ Ccol,acc_comp * __restrict__ C)
{
	int h,i,j=0,k=1,l=0,n=0;
	//calc second row
	for(i=0; i<(N-1); i++)
	{
		(C+n)->r = (Crow+n)->r - ( (B->r) * ((A+j)->r) - (B->im) * ((A+j)->im) ) + ( ((B+P2)->r) * ((A+M2+j)->r) - ((B+P2)->im) * ((A+M2+j)->im) );
		(C+n)->im = (Crow+n)->im - ( (B->r) * ((A+j)->im) + (B->im) * ((A+j)->r) ) + ( ((B+P2)->r) * ((A+M2+j)->im) + ((B+P2)->im) * ((A+M2+j)->r) );
		n++;j=j+M;
	}
	j=P;
	//calc second col
	for(i=0; i<(Q-2); i++)
	{
		(C+n)->r = (Ccol+k)->r - ( ((B+j)->r) * (A->r) - ((B+j)->im) * (A->im) ) + ( ((B+P2+j)->r) * ((A+M2)->r) - (B+P2+j)->im * (A+M2)->im );
		(C+n)->im = (Ccol+k)->im - ( ((B+j)->r) * (A->im) + ((B+j)->im) * (A->r) ) + ( ((B+P2+j)->r) * ((A+M2)->im) + (B+P2+j)->im * (A+M2)->r );
		n=n+(N-1);k++;j=j+P;
	}

	//calc rest of the 11x11 elements
	//C is stored in row major format
	n=N;i=P;j=M;
	for(h=0; h<(Q-2); h++)		//computes columns
	{
		for(k=0; k<(N-2); k++)	//computes row
		{
			(C+n)->r = (C+l)->r - ( ((B+i)->r) * ((A+j)->r) - ((B+i)->im) * ((A+j)->im) ) + ( ((B+P2+i)->r) * ((A+M2+j)->r) - ((B+P2+i)->im) * ((A+M2+j)->im) );
			(C+n)->im = (C+l)->im - ( ((B+i)->r) * ((A+j)->im) + ((B+i)->im) * ((A+j)->r) ) + ( ((B+P2+i)->r) * ((A+M2+j)->im) + ((B+P2+i)->im) * ((A+M2+j)->r) );
			n++;l++;j=j+M;
		}
		n++;l++;i=i+P;j=M;
	}
}


int main()
{

	float * data_ptr= malloc(sizeof(*data_ptr));
	matrix_multiplication_hps(data_ptr);
	return 0;
}
