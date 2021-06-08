/**********************************************************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: 
 * Author     : w00449361
 * Create     : 
 **********************************************************************************************************************/
#ifndef DRV_FPGA_H
#define DRV_FPGA_H

#include "memy.h"
#include "utils.h"

#define FPGA_BASE_ADDR 0x68000000 /* FPGA空间基地址 */

#define SIZE_64MB 0x04000000
#define FPGA_SIZE SIZE_64MB


#ifndef BYTE_SWAP32
#define BYTE_SWAP32(value) ((((unsigned)(value)&0x000000ff) << 24) | \
                            (((unsigned)(value)&0x0000ff00) << 8) |            \
                            (((unsigned)(value)&0x00ff0000) >> 8) |            \
                            (((unsigned)(value)&0xff000000) >> 24))
#endif

#ifndef BYTE_SWAP16
#define BYTE_SWAP16(value) ((((unsigned)(value)&0x00ff) << 8) | \
                            (((unsigned)(value)&0xff00) >> 8))
#endif


U16 FPGA_ReadReg(U32 ulChipIndex, U32 ulAddr);
void FPGA_WriteReg(U32 ulChipIndex, U32 ulAddr, U16 usData);
U16 FPGA_ReadReg_Lego(U32 ulChipIndex, U32 ulAddr);
void FPGA_WriteReg_Lego(U32 ulChipIndex, U32 ulAddr, U16 usData);

#endif
