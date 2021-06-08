/**********************************************************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description:
 * Author     : w00449361
 * Create     :
 **********************************************************************************************************************/
#include "espi_fpga.h"
#include "espi_reg_fpga.h"
#include "memy.h"
#include <time.h>

extern U32 g_espiFpgaMapAddr[];
#define FPGA_ESPI_READ_REG(cs, addr) FPGA_ReadReg_Lego(0, (g_espiFpgaMapAddr[cs] + addr))
#define FPGA_ESPI_WRITE_REG(cs, addr, val) FPGA_WriteReg_Lego(0, (g_espiFpgaMapAddr[cs] + addr), val)

#define sysUsDelay(x) delay_espi_us(x)

#define M_common_NULL_POINTER (-1)
#define FPGA_ESPI_R_WAITACK_TIMEUOT 1000

#define OS_EMBED_ASM __asm__ __volatile__
#define INLINE __attribute__((always_inline)) inline

INLINE void delay_espi_us(int cnt)
{
    for (int i = 0; i < 1000 * cnt; i++) {
        OS_EMBED_ASM("nop");
    }
}

U32 FPGA_Espi_WriteReg(U8 index, U32 addr, U32 data, U32 usseconds)
{
    U32 ret = 0;
    ret     = FPGA_EspiWriteReg_Lego_THD1(index, addr, data);
    return ret;
}
U32 FPGA_Espi_ReadReg(U8 index, U32 addr, U32 usseconds)
{

    U32 data = 0;
    (void)FPGA_EspiReadReg_Lego_THD1(index, addr, &data);

    return data;
}
U32 FPGA_Espi_Burst_WriteReg(U8 chipid, U32 regaddr, U8* dataBuffer, U8 opcode, U32 delay)
{
    U32 ret = 0;

    ret = FPGA_EspiBurstWriteReg_Lego_THD1(chipid, regaddr, dataBuffer, opcode);
    return ret;
}
U32 FPGA_Espi_Burst_ReadReg(U8 chipid, U32 regaddr, U8 opcode, U8* dataBuffer, U32 delay)
{
    U32 ret = 0;
    ret     = FPGA_EspiBurstReadReg_Lego_THD1(chipid, regaddr, dataBuffer, opcode);
    return ret;
}
U32 FPGA_EspiReset(U32 ulChipIndex, U16 rst)
{

    if (rst == 0) {
        rst = 0x2;
    }
    FPGA_ESPI_WRITE_REG(ulChipIndex, ESPI_RESET, rst);
    FPGA_ESPI_WRITE_REG(ulChipIndex, ESPI_RESET, 0);

    return 0;
}
U32 FPGA_EspiInit(U32 ulChipIndex)
{
    U32 tmp = 0;
    int i   = 0;
    // unlock
    FPGA_ESPI_WRITE_REG(ulChipIndex, ESPI_LOCK, 0x5a);

    /* 等待所有数据传输完毕。 */
    for (i = 0; i < FPGA_ESPI_R_WAITACK_TIMEUOT; i++) {
        if (0 != (FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS))) {
            break;
        } else {
            sysUsDelay(1);
        }
    }
    if (FPGA_ESPI_R_WAITACK_TIMEUOT <= i) {
        printf("FPGA_EspiInit_Lego timeout 1 \n");
        return (U32)ERROR;
    }

    tmp = FPGA_ESPI_READ_REG(ulChipIndex, ESPI_MODE);
    tmp = tmp & (0x0 << 4);  // master mode
    tmp = tmp & (0x0 << 3);  // slave 0：8比特模式；1：4比特模式
    tmp = tmp & (0x0 << 2);  // slave 0：CRC 关闭；1：CRC 开启
    tmp = tmp | (0x1 << 1);  // master 0：8比特模式；1：4比特模式
    tmp = tmp & (0x0 << 0);  // master 0：CRC 关闭；1：CRC 开启
    FPGA_ESPI_WRITE_REG(ulChipIndex, ESPI_MODE, tmp);

    // lock
    FPGA_ESPI_WRITE_REG(ulChipIndex, ESPI_LOCK, 0x0);

    // reset after init
    (void)FPGA_EspiReset(ulChipIndex, 0x2);

    return (U32)OK;
}
U32 FPGA_EspiBurstWriteReg_Lego_THD1(U32 ulChipIndex, U32 ulWriteAddr, U8* ulDataBuffer, U8 opcode)
{
    U32 ulRet;

    // clock_gettime(CLOCK_REALTIME, &g_timeLastTxTmp1);
    opcode_reg_u8 OpCodeTemp;
    OpCodeTemp.a = opcode;

    U16 temp = 0;
    S32 icnt = 0;

    if (NULL == ulDataBuffer) {
        return M_common_NULL_POINTER;
    }


    // #set used
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    if (temp & 0x1) {
        printf("THD1 using. reject.\n");
    } else {
        temp = temp | 0x1;
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    }
    /*
    // #opcode
    if(ulLength == 1){
        temp = 0x00;
    }else if (ulLength == 2){
        temp = 0x01;
    }else if (ulLength == 4){
        temp = 0x10;
    }else if (ulLength == 8){
        temp = 0x11;
    }
    else{
        printf("illegal transmit length in byte.\n");
        temp = FPGA_ESPI_READ_REG(0, THD1_STATUS(ulChipIndex));
        temp = temp ^ 0x1; //release
        FPGA_ESPI_WRITE_REG(0, THD1_STATUS(ulChipIndex), temp);
        return (U32)ERROR;
    }

    OpCodeTemp.s.burst_length = 0x0; // 0x0: 1 unit；0x1: 2 units；… 0xF：16 unit
    OpCodeTemp.s.burst_type = 0x1; // 0：fix mode；1：incremental mode
    OpCodeTemp.s.burst_with = temp; // 00：字节；01：半字；10：单字；11：双字。1 字 = 4 字节。
    OpCodeTemp.s.r_w_flag = 0x0; // 0：写模式；1：读模式
    */
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, OpCodeTemp.a);
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, 0x0);
    // #addr
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulWriteAddr >> 0) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulWriteAddr >> 8) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulWriteAddr >> 16) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulWriteAddr >> 24) & 0xff));
    // #send data
    for (int ii = 0; ii < (OpCodeTemp.s.burst_length + 1) * (1 << OpCodeTemp.s.burst_with); ii = ii + 4) {
#ifdef BIG_ENDIAN
        FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ulDataBuffer[ii + 3]);
        FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ulDataBuffer[ii + 2]);
        FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ulDataBuffer[ii + 1]);
        FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ulDataBuffer[ii + 0]);
#else
        FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ulDataBuffer[ii + 0]);
        FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ulDataBuffer[ii + 1]);
        FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ulDataBuffer[ii + 2]);
        FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ulDataBuffer[ii + 3]);
#endif
    }
    // #set ready
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    temp = temp & ~(0x1 << 1);  // 将寄存器0x0e[1]写为0
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    sysUsDelay(1);
    temp = temp | (0x1 << 1);  // 将寄存器0x0e[1]写为1；
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);

    do {
        temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
        if (temp & (0x1 << 2)) {
            break;
        }
        icnt++;
        sysUsDelay(1);
    } while (icnt <= 10000);

    if (icnt > 10000) {
        printf("FPGA_EspiBurstWriteReg_Lego_THD1 timeout! %x, %x, %x\n", ulChipIndex, ulWriteAddr, opcode);
        return -1;
    }
    // #release used
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    temp = temp & ~(0x1);  // 将寄存器0x0e[0]写为0
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);

    return OK;
}
U32 FPGA_EspiBurstReadReg_Lego_THD1(U32 ulChipIndex, U32 ulReadAddr, U8* pulDataBuffer, U8 opcode)
{
    U32 ulRet;

    opcode_reg_u8 OpCodeTemp;
    OpCodeTemp.a = opcode;

    U16 temp = 0;
    S32 icnt = 0;

    if (NULL == pulDataBuffer) {
        return M_common_NULL_POINTER;
    }

    // #set used
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    if (temp & 0x1) {
        printf("THD1 using. reject.\n");
    } else {
        temp = temp | 0x1;
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    }
    // #opcode
    /*
    if(ulLength == 1){
        temp = 0x00;
    }else if (ulLength == 2){
        temp = 0x01;
    }else if (ulLength == 4){
        temp = 0x10;
    }else if (ulLength == 8){
        temp = 0x11;
    }
    else{
        printf("illegal transmit length in byte.\n");
        temp = FPGA_ESPI_READ_REG(0, THD1_STATUS(ulChipIndex));
        temp = temp ^ 0x1; //release
        FPGA_ESPI_WRITE_REG(0, THD1_STATUS(ulChipIndex), temp);
        return (U32)ERROR;
    }
    OpCodeTemp.s.burst_length = 0x0; // 0x0: 1 unit；0x1: 2 units；… 0xF：16 unit
    OpCodeTemp.s.burst_type = 0x1; // 0：fix mode；1：incremental mode
    OpCodeTemp.s.burst_with = temp; // 00：字节；01：半字；10：单字；11：双字。1 字 = 4 字节。
    OpCodeTemp.s.r_w_flag = 0x0; // 0：写模式；1：读模式
    */
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, OpCodeTemp.a);
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, 0x0);
    // #addr
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulReadAddr >> 0) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulReadAddr >> 8) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulReadAddr >> 16) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulReadAddr >> 24) & 0xff));
    // #set ready
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    temp = temp & ~(0x1 << 1);  // 将寄存器0x0e[1]写为0
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    sysUsDelay(50);
    temp = temp | (0x1 << 1);  // 将寄存器0x0e[1]写为1；
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    do {
        temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
        if (temp & (0x1 << 2)) {
            break;
        }
        icnt++;
        sysUsDelay(10);
    } while (icnt <= 10000);
    if (icnt > 10000) {
        printf("FPGA_EspiBurstReadReg_Lego_THD1 timeout! %x, %x, %x\n", ulChipIndex, ulReadAddr, opcode);
        return -1;
    }
    // # 读回数据
    for (int ii = 0; ii < ((OpCodeTemp.s.burst_length + 1) * (1 << OpCodeTemp.s.burst_with)); ii = ii + 4) {
        // FPGA_ESPI_WRITE_REG(0, THD1_RD_POS(ulChipIndex), 0x1);
        // *(pulDataBuffer + ((OpCodeTemp.s.burst_length + 1)  * (2 << OpCodeTemp.s.burst_with)) - ii - 1) = (FPGA_ESPI_READ_REG(0,
        // MAS_THD1_OUT_L(ulChipIndex)) & 0xff); FPGA_ESPI_WRITE_REG(0, THD1_RD_POS(ulChipIndex), 0x0);
        // *(pulDataBuffer + ((OpCodeTemp.s.burst_length + 1)  * (2 << OpCodeTemp.s.burst_with)) - ii - 2) = (FPGA_ESPI_READ_REG(0,
        // MAS_THD1_OUT_H(ulChipIndex)) & 0xff);
#ifdef BIG_ENDIAN
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x1);
        pulDataBuffer[ii + 3] = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_L) & 0xff);
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x0);
        pulDataBuffer[ii + 2]  = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_H) & 0xff);
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x1);
        pulDataBuffer[ii + 1]  = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_L) & 0xff);
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x0);
        pulDataBuffer[ii]  = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_H) & 0xff);
#else
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x1);
        *(pulDataBuffer + 0) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_L) & 0xff);
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x0);
        *(pulDataBuffer + 1) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_H) & 0xff);
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x1);
        *(pulDataBuffer + 2) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_L) & 0xff);
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x0);
        *(pulDataBuffer + 3) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_H) & 0xff);
#endif
    }

    // #release used
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    temp = temp & ~(0x1);  // 将寄存器0x0e[0]写为0
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);

    return OK;
}
U32 FPGA_EspiWriteReg_Lego_THD1(U32 ulChipIndex, U32 ulWriteAddr, U32 ulDataBuffer)
{
    U32 ulRet;

    opcode_reg_u8 OpCodeTemp;
    OpCodeTemp.a = 0;

    U16 temp = 0;
    S32 icnt = 0;
    int i;

    // #set used
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    if (temp & 0x1) {
        printf("THD1 using. reject.\n");
    } else {
        temp = temp | 0x1;
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    }
    // #opcode
    OpCodeTemp.s.burst_length = 0x0;  // 0x0: 1 unit；0x1: 2 units；… 0xF：16 unit
    OpCodeTemp.s.burst_type   = 0x1;  // 0：fix mode；1：incremental mode
    OpCodeTemp.s.burst_with   = 0x2;  // 00：字节；01：半字；10：单字；11：双字。1 字 = 4 字节。
    OpCodeTemp.s.r_w_flag     = 0x0;  // 0：写模式；1：读模式
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, OpCodeTemp.a);
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, 0x0);
    // #addr
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulWriteAddr >> 0) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulWriteAddr >> 8) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulWriteAddr >> 16) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulWriteAddr >> 24) & 0xff));
    // #send data
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulDataBuffer >> 0) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulDataBuffer >> 8) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulDataBuffer >> 16) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulDataBuffer >> 24) & 0xff));
    // #set ready
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    temp = temp & ~(0x1 << 1);  // 将寄存器0x0e[1]写为0
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    sysUsDelay(1);
    temp = temp | (0x1 << 1);  // 将寄存器0x0e[1]写为1；
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    do {
        temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
        if (temp & (0x1 << 2)) {
            break;
        }
        icnt++;
        sysUsDelay(1);
    } while (icnt <= 100);
    if (icnt > 100) {
        printf("FPGA_EspiWriteReg_Lego_THD1 write timeout! 0x%x, 0x%x, 0x%x\n", ulChipIndex, ulWriteAddr, ulDataBuffer);
        return -1;
    }
    // #release used
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    temp = temp & ~(0x1);  // 将寄存器0x0e[0]写为0
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);

    return OK;
}
U32 FPGA_EspiReadReg_Lego_THD1(U32 ulChipIndex, U32 ulReadAddr, U32* pulDataBuffer)
{
    opcode_reg_u8 OpCodeTemp;
    OpCodeTemp.a = 0;

    S32 icnt = 0;

    U16 temp = 0;
    int i;

    if (NULL == pulDataBuffer) {
        return M_common_NULL_POINTER;
    }

    // #set used
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    if (temp & 0x1) {
        printf("THD1 using. reject.\n");
        return -1;
    } else {
        temp = temp | 0x1;
        FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    }
    // #opcode
    OpCodeTemp.s.burst_length = 0x0;  // 0x0: 1 unit；0x1: 2 units；… 0xF：16 unit
    OpCodeTemp.s.burst_type   = 0x1;  // 0：fix mode；1：incremental mode
    OpCodeTemp.s.burst_with   = 0x2;  // 00：字节；01：半字；10：单字；11：双字。1 字 = 4 字节。
    OpCodeTemp.s.r_w_flag     = 0x1;  // 0：写模式；1：读模式
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, OpCodeTemp.a);
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, 0x0);
    // #addr
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulReadAddr >> 0) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulReadAddr >> 8) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_L, ((ulReadAddr >> 16) & 0xff));
    FPGA_ESPI_WRITE_REG(ulChipIndex, MAS_THD1_IN_H, ((ulReadAddr >> 24) & 0xff));
    // #set ready
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    temp = temp & ~(0x1 << 1);  // 将寄存器0x0e[1]写为0
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    sysUsDelay(50);
    temp = temp | (0x1 << 1);  // 将寄存器0x0e[1]写为1；
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);
    do {
        temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
        if (temp & (0x1 << 2)) {
            break;
        }
        icnt++;
        sysUsDelay(10);
    } while (icnt <= 100);
    if (icnt >= 100) {
        printf("FPGA_EspiReadReg_Lego_THD1 read timeout! 0x%x, 0x%x\n", ulChipIndex, ulReadAddr);
        return -1;
    }
    // # 读回数据
#ifdef BIG_ENDIAN
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x1);
    *((U8*)pulDataBuffer + 3) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_L) & 0xff);
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x0);
    *((U8*)pulDataBuffer + 2) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_H) & 0xff);
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x1);
    *((U8*)pulDataBuffer + 1) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_L) & 0xff);
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x0);
    *((U8*)pulDataBuffer + 0) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_H) & 0xff);
#else
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x1);
    *((U8*)pulDataBuffer + 0) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_L) & 0xff);
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x0);
    *((U8*)pulDataBuffer + 1) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_H) & 0xff);
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x1);
    *((U8*)pulDataBuffer + 2) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_L) & 0xff);
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_RD_POS, 0x0);
    *((U8*)pulDataBuffer + 3) = (FPGA_ESPI_READ_REG(ulChipIndex, MAS_THD1_OUT_H) & 0xff);
#endif
    // printf("%x, %x, *pulDataBuffer: 0x%x \n", ulChipIndex, ulReadAddr, *((U32*)pulDataBuffer));
    // #release used
    temp = FPGA_ESPI_READ_REG(ulChipIndex, THD1_STATUS);
    temp = temp & (0xfffffffe);  // 将寄存器0x0e[0]写为0
    FPGA_ESPI_WRITE_REG(ulChipIndex, THD1_STATUS, temp);

    return OK;
}
