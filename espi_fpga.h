/**********************************************************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: 
 * Author     : w00449361
 * Create     : 
 **********************************************************************************************************************/
#ifndef ESPI_DRV_FPGA_H
#define ESPI_DRV_FPGA_H
 
#include "espi_reg_fpga.h"
#include "memy.h"



// 类型定义
typedef union opcode_reg_value_ {
    U8 a;
    struct opcode_reg_s {
        U8 r_w_flag     : 1;    /* 读写指示：0：指示当前操作为写操作；1：指示当前操作为读操作； */
        U8 burst_with   : 2;    /* 长度单位指示，指示length域表示的长度的单位：00：长度单位为字节；01：长度单位为半字；10：长度单位为字；11：长度单位为双字； */
        U8 burst_type   : 1;    /* 指示burst传输时地址增长模式：0：fix地址模式1：INCR地址模式； */
        U8 burst_length : 4;    /* 总线上传输的数据个数，单位由width域指示，通常与awlen/arlen 长度对应：0x0：传输1个单位数据；0x1：传输2个单位数据；…0xF：传输16个单位数据； */
    } s;
}opcode_reg_u8;

U32 FPGA_Espi_WriteReg(U8 index, U32 addr, U32 data, U32 usseconds);
U32 FPGA_Espi_ReadReg(U8 index,  U32 addr, U32 usseconds);
U32 FPGA_Espi_Burst_WriteReg(U8 chipid, U32 regaddr, U8 *dataBuffer, U8 opcode, U32 delay);
U32 FPGA_Espi_Burst_ReadReg(U8 chipid, U32 regaddr, U8 opcode, U8 *dataBuffer, U32 delay);

U32 FPGA_EspiReset_Lego(U32 ulChipIndex, U16 rst);
U32 FPGA_EspiInit_Lego(U32 ulChipIndex);
U32 FPGA_EspiBurstWriteReg_Lego_THD1(U32 ulChipIndex, U32 ulWriteAddr, U8 *ulDataBuffer, U8 opcode);
U32 FPGA_EspiBurstReadReg_Lego_THD1(U32 ulChipIndex, U32 ulReadAddr, U8 *pulDataBuffer, U8 opcode);
U32 FPGA_EspiWriteReg_Lego_THD1(U32 ulChipIndex, U32 ulWriteAddr, U32 ulDataBuffer);
U32 FPGA_EspiReadReg_Lego_THD1(U32 ulChipIndex, U32 ulReadAddr, U32 *pulDataBuffer);


#endif
