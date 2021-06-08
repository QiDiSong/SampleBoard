/**********************************************************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: 
 * Author     : w00449361
 * Create     : 
 **********************************************************************************************************************/
#ifndef MEM_DRV_FPGA_H
#define MEM_DRV_FPGA_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "utils.h"

#include <sys/mman.h>

#define MEM_DEV			 "/dev/mem"
#define MAP_MEM_BASE	 (0x68000000)
#define PAGE_SIZE		 (0x1000)
#define MAP_SIZE		 (PAGE_SIZE)

#define MIPI_FPGA_MODULE_CNT (1)
#define ESPI_FPGA_MODULE_CNT (2)

#define BIG_ENDIAN

#define MAP_GLOBAL



int mReadRegAbsoluteAddr(U16 * value, U32 addr);
int mWriteRegAbsoluteAddr(U32 addr, U16 value);

int fpgaMemMap(U32 addrPhy, U32* addrVirt);
int fpgaMemUnmap(int dev_fd, U32* addrVirt);

#endif

