/**********************************************************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: 
 * Author     : w00449361
 * Create     : 
 **********************************************************************************************************************/
#ifndef MIPI_DRV_FPGA_H
#define MIPI_DRV_FPGA_H

#include "utils.h"

int MipiParsingFileAndSend(char* configFile, U32 chann, U32 startAddr);

void Mipi_WriteReg(U32 ulChipIndex, U32 chann, U32 ulAddr, U32 ulVal);
U32 Mipi_ReadReg(U32 ulChipIndex, U32 chann, U32 ulAddr);

#endif
