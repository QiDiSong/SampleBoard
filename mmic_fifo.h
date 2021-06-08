/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: common.c
 * Author: none
 * Create: 2020-05-16
 */
#ifndef MMIC_DRV_FIFO_RPU_H
#define MMIC_DRV_FIFO_RPU_H

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
// #include "Common/Std_Types.h"
#include "utils.h"
/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

#define PACKET_CNT_QUEUE        (1024)

#define CMD_READ    0x0
#define CMD_WRITE    0x1
typedef unsigned int uint32;



typedef union {
    struct {
        uint32 chipId   :2;
        uint32 opFlag   :1;
        uint32 reserved :29;
    }bits;
    uint32    flags;
} OpFlags;

typedef struct {
    OpFlags cmdInfo;
    uint32 addrRegMmic;
    uint32 valRegMmic;
}PacketDataType;


typedef struct {
	PacketDataType *ppacketData;
	uint32 *pfront; //指向队头的索引，这个所指的空间不存放元素
	uint32 *prear; //指向队尾的索引，存放最后一个元素
    uint32 startAddrRpu;
    uint32 frontAddrRpu;
    uint32 rearAddrRpu;
}PacketDataQueueType;


#if 0
void InitQueue(PacketDataQueueType *pCQ);
int IsQueueEmpty(PacketDataQueueType *pCQ);
int IsQueueFull(PacketDataQueueType *pCQ);
int PushElement(PacketDataQueueType *pCQ);
int PopElement(PacketDataQueueType *pCQ);
int GetHeadElement(PacketDataQueueType *pCQ, PacketDataType **pData);
int GetRearElement(PacketDataQueueType *pCQ, PacketDataType **pData);
#endif
int IsQueueFull(U32 front, U32 rear);

extern PacketDataQueueType g_InputDataQueue;
extern PacketDataQueueType g_OutputDataQueue;


#endif
