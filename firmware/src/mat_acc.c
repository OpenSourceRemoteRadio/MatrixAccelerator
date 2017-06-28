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
/*
 * accelerator.c
 *
 */


/******************************************************************************
 * Include public/global Header files
******************************************************************************/

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
#include <sys/time.h>

/******************************************************************************
 * Include private Header files
******************************************************************************/

#include "mat_acc_firmware.h"
//#include "alt_types.h"
#include "mat_acc.h"

extern acc_comp *A, *B, *C;
extern acc_comp *Crow, *Ccol;
//extern acc_comp *ACCrow, *ACCcol;

volatile unsigned int * CAPBRIDGE_ADDR_ptr;
volatile unsigned int * timer_memmaped;

volatile unsigned int * ddr_memmaped;

volatile unsigned int * pb_msgdma_1_desc;
volatile unsigned int * pb_msgdma_2_desc;
int kflag;

/**
 * @ingroup Matrix_accelerator
 * @brief Computes Matrix multiplication in HPS using NEON optimised library. This function first computes first row and first column of the resultant matrix and then computes the remaining elements of resultant matrix. Computes the time taken for computing first row and first column of matrix and writes the time taken in last few offsets of data_ptr memory
 * @param data_ptr Memory to write the resultant matrix elements. First 13 words are first Row elements followed by 12 first column elements and remaining elements are stored in row major format.
 */

void matrix_multiplication_hps(float *data_ptr)
{
	int iteration_count= 0;
	int memory_index= 0;
	unsigned int *data_ptr_uint32 = (unsigned int *)data_ptr;

	printf("HPS MATRIX multiplication\n");

#ifdef ANNOTATE_ENABLE
	ANNOTATE_COLOR(ANNOTATE_PURPLE, "HPS Start\n");

	ANNOTATE_CHANNEL_END(0);
#endif

#ifdef TIMER_CONFIG
	timer_memmaped[2]= 0xffffffff;
	timer_memmaped[3]= 0xffffffff;
	timer_memmaped[4]= 0xffffffff;
	timer_memmaped[5]= 0xffffffff;

	timer_memmaped[1] = BIT_ENABLE(2)|BIT_ENABLE(1);
#endif

	Bt_x_A_single_row(B,A,Crow);
	Bt_x_A_single_column(B,A,Ccol);

#ifdef TIMER_CONFIG
	timer_memmaped[6]= 0x1;
	printf("Timer Value %X %X %X %X\n",timer_memmaped[9],timer_memmaped[8],timer_memmaped[7],timer_memmaped[6]);
#endif

#ifdef ANNOTATE_ENABLE
	ANNOTATE_COLOR(ANNOTATE_PURPLE, "HPS done\n");

	ANNOTATE_CHANNEL_END(0);
#endif

	/*data_ptr_uint32[340] = timer_memmaped[9];
	data_ptr_uint32[341] = timer_memmaped[8];
	data_ptr_uint32[342] = timer_memmaped[7];
	data_ptr_uint32[343] = timer_memmaped[6];*/

#ifdef TIMER_CONFIG
	data_ptr_uint32[340] = (timer_memmaped[9]<<16) | (timer_memmaped[8]);
	data_ptr_uint32[341] = (timer_memmaped[7]<<16) | (timer_memmaped[6]);
#endif

	printf("Crow\n");

	for (iteration_count = 0; iteration_count < 13; ++iteration_count)
	{
		printf("CRow [%d] = Real %f Imag %f\n",iteration_count,Crow[iteration_count].r ,Crow[iteration_count].im);
	}

	printf("Ccol\n");

	for (iteration_count = 0; iteration_count < 13; ++iteration_count)
	{
		printf("CCol [%d] = Real %f Imag %f\n",iteration_count,Ccol[iteration_count].r ,Ccol[iteration_count].im);
	}

	remaining_element_comp(A, B, Crow, Ccol, C);

	for (iteration_count = 0,memory_index= 0; iteration_count < 13; ++iteration_count,memory_index = memory_index+2)
	{
		data_ptr[memory_index] = Crow[iteration_count].r;
		data_ptr[memory_index+1] = Crow[iteration_count].im;
	}

	//for (iteration_count = 0,memory_index= 26; iteration_count < 13; ++iteration_count,memory_index = memory_index+2)
	for (iteration_count = 0; iteration_count < 13; ++iteration_count,memory_index = memory_index+2)
	{
		data_ptr[memory_index] = Ccol[iteration_count].r;
		data_ptr[memory_index+1] = Ccol[iteration_count].im;
	}

	//for (iteration_count = 0,memory_index= 52; iteration_count < (144); ++iteration_count,memory_index = memory_index+2)
	for (iteration_count = 0; iteration_count < (144); ++iteration_count,memory_index = memory_index+2)
	{
		data_ptr[memory_index] = C[iteration_count].r;
		data_ptr[memory_index+1] = C[iteration_count].im;
	}

}
