

/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: common.c
 * Author: none
 * Create: 2020-05-16
 */

#include "mmic_fifo.h"

#if 0
/*
 * PacketDataQueue structure and operations
 *
 * */
void InitQueue(PacketDataQueueType *pCQ)
{
	 *(pCQ->pfront) = 0x0u;
     *(pCQ->prear) = 0x0u;
     return;
}

int IsQueueEmpty(PacketDataQueueType *pCQ)
{
	if (*(pCQ->pfront) == *(pCQ->prear))
		return 1;
	else
		return 0;
}

int IsQueueFull(PacketDataQueueType *pCQ)
{
	if (*((pCQ->prear) + 1) % PACKET_CNT_QUEUE == *(pCQ->pfront))
		return 1;
	else
		return 0;
}

int PushElement(PacketDataQueueType *pCQ)
{
	if (IsQueueFull(pCQ))
		return -1;

	*(pCQ->prear) = (*(pCQ->prear) + 1) % PACKET_CNT_QUEUE;

	return 0;
}

int PopElement(PacketDataQueueType *pCQ)
{
	if (IsQueueEmpty(pCQ))
		return -1;

	*(pCQ->pfront) = (*(pCQ->pfront) + 1) % PACKET_CNT_QUEUE;
	return 0;
}
int GetHeadElement(PacketDataQueueType *pCQ, PacketDataType **pData)
{
	if (IsQueueEmpty(pCQ))
		return -1;

	*pData = (PacketDataType*)&(pCQ->ppacketData[(*(pCQ->pfront) + 1) % PACKET_CNT_QUEUE]);
	return 0;
}

int GetRearElement(PacketDataQueueType *pCQ, PacketDataType **pData)
{
	if (IsQueueFull(pCQ))
		return -1;

	*pData = (PacketDataType*)&(pCQ->ppacketData[(*(pCQ->prear) + 1) % PACKET_CNT_QUEUE]);
	return 0;
}
#endif


int IsQueueFull(U32 front, U32 rear)
{
	if ((rear + 1) % PACKET_CNT_QUEUE == front)
		return 1;
	else
		return 0;
}

// int PushElement()
// {
// 	if (IsQueueFull(pCQ))
// 		return -1;

// 	//*(pCQ->prear) = (*(pCQ->prear) + 1) % PACKET_CNT_QUEUE;

// 	//send to RPU

// 	return 0;
// }