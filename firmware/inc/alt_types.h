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
*//**
* @file alt_types.h
* @brief Header file for the altera datatype
*/
#ifndef __ALT_TYPES_H__
#define __ALT_TYPES_H__
/* 
 * Don't declare these typedefs if this file is included by assembly source.
 */

/*******************************************************************************
* Include public/global header files
*******************************************************************************/
#include <stdbool.h>
/*******************************************************************************
* Include private header files
*******************************************************************************/


/******************************************************************************
 * Section for #define's & typedef's
 ******************************************************************************/
#define MAX_SERVER_COUNT 2
#define MAX_QUEUE_NODES 30  //0 and 1 are not acceptable

#define TCP_PACKET_HEADER_STRING 		"alt"
#define TCP_PACKET_HEADER_STRING_SIZE   (sizeof(TCP_PACKET_HEADER_STRING)-1)
#define TCP_PACKET_HEADER_SIZE 			sizeof(Packet_Header)
#define Hepta_A10_2x4_msgdma_ACCELERATOR_MSGDMA_INSTANCE_INDEX 0
#define Hepta_A10_2x4_msgdma_CSR_RESOURCE_INDEX 0
#define Hepta_A10_2x4_msgdma_DESCRIPTOR_SLAVE_RESOURCE_INDEX 1

#define Hepta_A10_2x4_msgdma_CONFIG_COUNT 0
#define Hepta_A10_2x4_msgdma_SERVICE_COUNT 0


#ifndef ALT_ASM_SRC
typedef signed char  alt_8;
typedef unsigned char  alt_u8;
typedef signed short alt_16;
typedef unsigned short alt_u16;
typedef signed int alt_32;
typedef unsigned int alt_u32;
typedef signed long alt_64;
typedef unsigned long alt_u64;
typedef volatile unsigned int * bus_address;

#endif

#define ALT_INLINE        __inline__
#define ALT_ALWAYS_INLINE __attribute__ ((always_inline))
#define ALT_WEAK          __attribute__((weak))

/******************************************************************************
 * Section for enums
 ******************************************************************************/

/******************************************************************************
 * Section for struct definitions
 ******************************************************************************/
/**
 * @ingroup CommandManager
 * @brief Tells the decoder in which module the operation to be performed
 * */


typedef enum
{
    MODULE_INDEX_SYSID=0,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_CPRI=1,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_SPI=2,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_CAPCTRL=3,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_JESDTX=4,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_JESDRX=5,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_DOWNLINK=6,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_REGMAP_DFD=7,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_UPLINK=8,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_DPD_REG_MAP_DFD=9,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_DPD_REG_MAP=10,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_ILC=11,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_TIMER=12,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_HEPTA_DPD_XCORR=13,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_PTX=14,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_POBSRX=15,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_CTX_WRITE=16,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_COBSRX_WRITE=17,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_TSE_TRANSMIT=18,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_TSE_CAPTURE=19,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_ETHERNET=20,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_MM2ST=21,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_AVST_TO_AVMM=22,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_MSGDMA=23,    /**< Operation to be performed in the sysid module*/
    MODULE_INDEX_GOERTZEL_CAPTURE=24,    /**< Operation to be performed in the buffer_mem module*/
    MODULE_INDEX_GOERTZEL_SPECTRUM=25,    /**< Operation to be performed in the buffer_mem module*/
    MODULE_INDEX_COMMAND_MANAGER=26,    /**< To configure the Command Manager module*/
}Module_Index_Enum_Def;

typedef struct
{
    char Header_String[3];    /**< This is the first field of Ethernet Packet. Incoming packets will be checked for this string*/
    unsigned int Id:8;    /**< Unique Id for each incomming command. Supports 256 commands simultaenously.*/
    unsigned int Length;    /**< Number of bytes succeeding header. This includes Command and data.  Maximum 65536 bytes of command and data transfer possible*/
    unsigned char server_index;
    char pData[0];    /**< Points to the memory that has command header*/
}__attribute__((__packed__))Packet_Header,*pPacket_Header;

typedef struct
{
    unsigned int Transfer_Type:1;    /**< This field specifies whether command is polling or service type. */
    unsigned int Module_Index:7;    /**< Index of module to configure.  Maximum 128 modules can be configured*/
    unsigned int Operation_Type:4;    /**< Operation to be performed e.g Read, Write and Config. Supports maximum 16 operations.*/
    unsigned int Instance:4;    /**< Instance of module to configure.  Maximum 16 instances of each modules can be configured*/
    unsigned int Resource:8;    /**< Index of the resource to be accessed*/
    unsigned short Address;    /**< Offset address or index of configuration to be done. Maximum 65536 address/offsets can be configured*/
    unsigned int Bit_Mask;    /**< To mask resgiter data during write and read*/
    //unsigned short Length;    /**< Number of data offsets succeeding command header.  Maximum 65536 words of data transfer supported*/
    unsigned int Length:22;    /**< Number of data offsets succeeding command header.  Maximum 65536 words of data transfer supported*/
    //char reserved[3];
    unsigned int is_CPRI_CM:1;		/**<Decides if it is C&M packet*/
    unsigned int server_index:1;
    //unsigned int reserved:6;
    char pData[0];    			/**< Points to the memory that has data*/
}__attribute__((__packed__))Command_Header,*pCommand_Header;


/**
 * @ingroup CommandManager
 * @brief Type of the command to be transfered.
 */

typedef enum
{
	TRANSFER_TYPE_POLL =  0, /**< Command will be polled from the server and the client will respond for the command whenever it receives poll command. */
	TRANSFER_TYPE_SERVICE, /**< Command will be sent from the server and the client will register the operation in the async database and responds periodically which trigers callback registered in the server. */
}Transfer_Type_Enum_Def;

/**
 * @ingroup CommandManager
 * @brief Tells the decoder what to operation be performed
 * */


typedef enum
{
    OPERATION_TYPE_SET=0,    /**< set operation*/
    OPERATION_TYPE_GET=1,    /**< get operation*/
    OPERATION_TYPE_CONFIG=2,    /**< config operation*/
    OPERATION_TYPE_SERVICE=3,    /**< service operation*/
    OPERATION_TYPE_ERROR=4,    /**< Inform a error report to the server*/
    OPERATION_TYPE_ACK=5,    /**< Acknowledge to an action to the server*/
    OPERATION_TYPE_MANAGE=6,    /**< Command to manage the application*/
}Operation_Type_Enum_Def;


/**
 * @ingroup SocketManager
 * @brief Socket descpriptors.
 */

typedef struct
{
	int 	connectedfd 	; 					/**< Descriptor for the connected socket for read and write. */
	char * 	pserver_ip_address ; 				/**< Server IPv4 address */
	char * 	port_no      	;
	struct addrinfo	* addrinfo_ptr;
	bool	connected		;
	bool	connecting		;
	bool	reading			;
	unsigned int server_index;
}socket_handles;


/**
 * @ingroup CommandManager
 * @brief Link list to maintain the commands in the scheduler.
 */


typedef struct
{
	struct command_queue *Next;	/**< Pointer to the next element in queue.*/
	void * pQueueData;			/**< Command of the current queue element.*/
	unsigned int id;				/**< Id for the current command.*/
}command_queue,*pcommand_queue;

/**
 * @ingroup CommandManager
 * @brief Link list Heads to maintain the commands in the scheduler.
 */

typedef struct
{
	pthread_mutex_t queue_lock;		/**< Mutex lock to synchronise queueing and dequeuing .*/
	struct command_queue *Head;		/**< Pointer to the first command in the queue.*/
	char *queue_name;			/**< Name of the command queue.*/
	unsigned int num_of_nodes;			/**< Number of command elements in the queue.*/
}command_queue_head, *pcommand_queue_head;


/**
 * @ingroup CommandManager
 * @ingroup SocketManager
 * @brief Scheduler to maintain the firmware details.
 */

typedef struct
{
	command_queue_head        	WriteQueueHead;									/**< Queue head for commands to be sent to server*/
	command_queue_head        	CommandQueueHead;								/**< Queue head for valid commands to be executed*/
	pthread_t 		          	pServerConnectThreadHandle[MAX_SERVER_COUNT]; 	/**< Thread to perform write in the hardware modules*/
	pthread_t 		          	pTransportReadThreadHandle[MAX_SERVER_COUNT]; 	/**< Thread to perform write in the hardware modules*/
	pthread_t 		          	pTransportWriteThreadHandle; 					/**< Thread to perform read from the hardware modules*/
	pthread_cond_t	          	WriteThreadCondition;							/**< Condition signal to signal write thread*/
	pthread_t 		          	pCommandThreadHandle; 							/**< Thread to manage hardware modules*/
	pthread_cond_t            	CommandThreadCondition;							/**< Condition signal to signal command thread*/
	pthread_cond_t	          	condition_signal; 								/**< Condition to signal the read/write threads*/
	pthread_mutex_t           	lock; 											/**< Lock to synchronise the access to the fields in the scheduler header*/
	bool			          	is_thread		;								/**< Condition to proceed to next iteration in threads*/
	socket_handles	  		  	ServerClientDetails[MAX_SERVER_COUNT]; 			/**< Heptaconnection handles to get server client details*/
	unsigned int				number_of_server;								/**< Number of servers initiated*/
	bus_address        		  	ddrmemmapped;									/**< Virtual address for DDR3 memory*/
}socket_manager_header,*psocket_manager_header;

#define debug_print(X)  do{\
							printf("%s %d :",__FUNCTION__,__LINE__);\
							printf(X);\
						  }while(0);


#endif /* __ALT_TYPES_H__ */
