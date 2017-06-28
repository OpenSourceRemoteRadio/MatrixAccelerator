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
//#include "accelerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <stdbool.h>

/*******************************************************************************
* Define input matrix sizes
*******************************************************************************/
#define M 7168	//no. of rows of A
#define N 13 	//no. of colums of A

#define P M		//no. of rows of B	//for matrix multiplication M=P is reqd condition for (A^T*B)
#define Q 13	//no. of columns of B

#define MN ((M)*(N))
#define PQ ((P)*(Q))
#define P2 ((2)*(P)-(1))
#define M2 ((2)*(M)-(1))

#define INCREMENTAL_DATA
//#define USER_DEFINED_DATA
//#define DEBUG_MODE

#define MATRIX_SIZE_SAMPLES 14360
#define INPUT_BYTES ((MATRIX_SIZE_SAMPLES)*(4))

typedef struct
{
	float r,im;
}acc_comp;

/**
 * @fn void Bt_x_A_single_row(acc_comp * __restrict__ A,acc_comp * __restrict__ B,acc_comp * __restrict__ Crow)
 * @ingroup Matrix_accelerator
 * @brief This function is used for debug purposes. This function calculates first row elements in ARM and this result can be used to compare with accelerator output
 * @param A input matrix pointer
 * @param B input matrix pointer
 * @param Crow output row matrix pointer
 */
void Bt_x_A_single_row(acc_comp * __restrict__ A,acc_comp * __restrict__ B,acc_comp * __restrict__ Crow)
{
	int i, k, j = 0, n = 0;
	float tempr = 0, tempi=0;
	for (i = 0; i < MN; i=i+M) //i controls column wise shift for A matrix
	{
		for (k = 0; k < M; k++) //k < M or P
		{
			tempr += (((B+j)->r * (A+i+j)->r) - ((B+j)->im * (A+i+j)->im));
			tempi += (((B+j)->r * (A+i+j)->im) + ((B+j)->im * (A+i+j)->r));
			j++;											//j controls row wise shift for both A and B matrix

		}
		(Crow+n)->r = tempr;					//Output is stored in row major format
		(Crow+n)->im = tempi;

		n++;tempr = 0;tempi = 0;j = 0;
	}
}

/**
 * @fn void Bt_x_A_single_column(acc_comp * __restrict__ A,acc_comp * __restrict__ B,acc_comp * __restrict__ Ccol)
 * @ingroup Matrix_accelerator
 * @brief This function is used for debug purposes. This function calculates first column elements in ARM and this result can be used to compare with accelerator output
 * @param A input matrix pointer
 * @param B input matrix pointer
 * @param Ccol output column matrix pointer
 */
void Bt_x_A_single_column(acc_comp * __restrict__ A,acc_comp * __restrict__ B,acc_comp * __restrict__ Ccol)
{
	int  k, h, j = 0, n = 0;
	float tempr = 0, tempi=0;
	for (h = 0; h < PQ; h=h+P) //h controls column wise shift for B matrix
	{
		for (k = 0; k < M; k++) //k < M or P
		{
			tempr += (((B+h+j)->r * (A+j)->r) - ((B+h+j)->im * (A+j)->im));
			tempi += (((B+h+j)->r * (A+j)->im) + ((B+h+j)->im * (A+j)->r));
			j++;											//j controls row wise shift for both A and B matrix
		}
		(Ccol+n)->r = tempr;					//Output is stored in row major format
		(Ccol+n)->im = tempi;
		n++;tempr = 0;tempi = 0;j = 0;
	}
}
