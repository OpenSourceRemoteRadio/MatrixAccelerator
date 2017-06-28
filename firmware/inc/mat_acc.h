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
* @file accelerator_main.h
* @brief This file defines all the API associated with working of Matrix vector row accelerator to calculate [B^T X A] for two matrices of size [7k x 13]
* @ingroup Matrix_accelerator
*/

#ifndef ACCELERATOR_H_
#define ACCELERATOR_H_

#include "alt_types.h"
#define TIMER_CONFIG
//#define PB_INPUT

/*Can be changed*/
#ifdef PB_INPUT
#define PAT1SRC_ADDR 0xFF210000 			//pattern side HPS DDR address where one of the matrix is stored - 1st MSGDMA
#else
#define PAT1SRC_ADDR 0x20000000
#endif

#define PAT2SRC_ADDR 0x20010000					//pattern side HPS DDR address where the other matrix is stored- 2nd MSGDMA

#define PAT3SRC_ADDR 0x20020000					//pattern side HPS DDR address where one of the matrix is stored - 1st MSGDMA
#define PAT4SRC_ADDR 0x20030000					//pattern side HPS DDR address where the other matrix is stored- 2nd MSGDMA

#define CAPDEST_ADDR 0x30000000					//capture side HPS SDRAM address to collect the result


/*DO NOT CHANGE*/
#define MM2ST_CTRL1 0xff202000	//address of control register of pattern msgdma	1
#define MM2ST_CTRL2 0xff208000 	//address of control register of pattern msgdma 2
#define ST2MM_CTRL  0xff203000	//address of control register of capture msgdma

//#define ACCEL_STATUS  0xff205000	//address of control register of capture msgdma
#define MSGDMA_FREQUENCY 245760000  //307200000
#define DSP_FREQUENCY	 368640000 //245760000

#define PATDEST_ADDR 0x00000000					//address of PB buffer	//DO NOT CHANGE
#define CAPBRIDGE_ADDR 0xc0000000				//H2F bridge address to collect result from FPGA accelerator //DO NOT CHANGE

#define PAT_READ_STRIDE   1
#define PAT_WRITE_STRIDE  0
#define PAT_READ_BURST    64
#define PAT_WRITE_BURST   0

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

typedef struct
{
	unsigned int row_i;
	unsigned int row_q;
	unsigned int col_i;
	unsigned int col_q;
}result_row_col;

typedef struct
{
	unsigned int Read_Address;
	unsigned int Write_Address;
	unsigned int Length;
	unsigned int Control;
}msgdma_standard_descriptor;


extern volatile unsigned int * pb_msgdma_1_desc;
extern volatile unsigned int * pb_msgdma_2_desc;

extern volatile unsigned int * ddr_memmaped;
extern volatile unsigned int * timer_memmaped;

extern volatile unsigned int *pPattern_ctrl1, *pPattern_ctrl2, *pCapture_ctrl,*ACCEL_STATUS_ptr;
extern volatile unsigned int * CAPBRIDGE_ADDR_ptr;

/*Function declarations*/
int fileoperation(unsigned int* data_ptr,int ptr_length,int kflag);
int matmult_row1_col1(alt_u32 src_addr_A, alt_u32 src_addr_B, alt_u32 dest_addr_C);
int mat_mult(alt_u32 src_addr_A, alt_u32 src_addr_B, alt_u32 dest_addr_C);
int comp_h_mat (alt_u32 src_addr_H0,alt_u32 src_addr_H1,alt_u32 src_addr_H2);

void write_to_mem(off_t address_offset, int length);
//void mem_to_fpga(msgdma_dev *dev, alt_u32 src_addr_offset, alt_u32 dest_addr_offset, int length);
void fpga_to_mem(unsigned int *pCapture, int length);

void construct_input_matrix(acc_comp * __restrict__ A,acc_comp * __restrict__ B, off_t src_addr_A, off_t src_addr_B);
void remaining_element_comp(acc_comp * __restrict__ A,acc_comp * __restrict__ B,acc_comp * __restrict__ Crow, acc_comp * __restrict__ Ccol,acc_comp * __restrict__ C);
void Bt_x_A_single_row(acc_comp * __restrict__ A,acc_comp * __restrict__ B,acc_comp * __restrict__ Crow);
void Bt_x_A_single_column(acc_comp * __restrict__ A,acc_comp * __restrict__ B,acc_comp * __restrict__ Ccol);

void construct_A_matrix(acc_comp * __restrict__ A, float * virtual_addr_A);
void construct_B_matrix(acc_comp * __restrict__ B, float * virtual_addr_B);
void gen_scramble(unsigned int* pattern_source,unsigned int* data,int size);
void alt_avalon_msgdma_transfer(unsigned int *pb_msgdma_desc_ptr,unsigned int source_add,unsigned int dest_add,unsigned int size_in_bytes);
void matrix_multiplication_fpga(float *data_ptr);
void write_A_matrix_hps_ddr3(unsigned int * data_ptr,unsigned int length);
void write_B_matrix_hps_ddr3(unsigned int * data_ptr,unsigned int length);
void matrix_multiplication_hps(float *data_ptr);


#endif /* ACCELERATOR_H_ */
