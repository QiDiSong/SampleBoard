/**********************************************************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: 
 * Author     : w00449361
 * Create     : 
 **********************************************************************************************************************/
#ifndef ESPI_DRV_FPGA_REG_H
#define ESPI_DRV_FPGA_REG_H

#include "fpga_drv.h"

#define ESPI_MODULE_OFFSET (0x0b000)
#define ESPI_BASE_REG_ADDR(espi) (FPGA_BASE_ADDR + ESPI_MODULE_OFFSET + (espi)*0x1000)

#ifndef MAP_GLOBAL

#define ESPI_LOCK(espi)					  (0x00 + ESPI_BASE_REG_ADDR(espi))		  // 寄存器锁
#define ESPI_RESET(espi)                  (0x02 + ESPI_BASE_REG_ADDR(espi))       // 复位控制
#define ESPI_MODE(espi)                   (0x04 + ESPI_BASE_REG_ADDR(espi))       // 模式控制
#define ESPI_CRC_CLR(espi)                (0x06 + ESPI_BASE_REG_ADDR(espi))       // CRC错误清零
#define ESPI_TIMEOUT_CLR(espi)            (0x08 + ESPI_BASE_REG_ADDR(espi))       // 超时错误清零
#define THD1_RD_POS(espi)                 (0x0a + ESPI_BASE_REG_ADDR(espi))       // 控制读回数据为高8bit，还是低8bit
#define THD2_RD_POS(espi)                 (0x0c + ESPI_BASE_REG_ADDR(espi))       // 控制读回数据为高8bit，还是低8bit
#define THD1_STATUS(espi)                 (0x0e + ESPI_BASE_REG_ADDR(espi))       // 进程1相关状态显示
#define THD2_STATUS(espi)                 (0x10 + ESPI_BASE_REG_ADDR(espi))       // 进程2相关状态显示
#define ESPI_STATE(espi)                  (0x12 + ESPI_BASE_REG_ADDR(espi))       // ESPI 状态机
#define ESPI_OPCODE(espi)                 (0x14 + ESPI_BASE_REG_ADDR(espi))       // 当前传输操作码
#define ESPI_ADDR_3(espi)                 (0x16 + ESPI_BASE_REG_ADDR(espi))       // 当前传输地址[31:24]
#define ESPI_ADDR_2(espi)                 (0x18 + ESPI_BASE_REG_ADDR(espi))       // 当前传输地址[23:16]
#define ESPI_ADDR_1(espi)                 (0x1a + ESPI_BASE_REG_ADDR(espi))       // 当前传输地址[15:8]
#define ESPI_ADDR_0(espi)                 (0x1c + ESPI_BASE_REG_ADDR(espi))       // 当前传输地址[7:0]
#define MAS_THD1_IN_H(espi)               (0x1e + ESPI_BASE_REG_ADDR(espi))       // 主模式进程1传输的数据[15:8]
#define MAS_THD1_IN_L(espi)               (0x20 + ESPI_BASE_REG_ADDR(espi))       // 主模式进程1传输的数据[7:0]
#define MAS_THD1_OUT_H(espi)              (0x22 + ESPI_BASE_REG_ADDR(espi))       // 主模式进程1接收的数据[15:8]
#define MAS_THD1_OUT_L(espi)              (0x24 + ESPI_BASE_REG_ADDR(espi))       // 主模式进程1接收的数据[7:0]
#define MAS_THD2_IN_H(espi)               (0x26 + ESPI_BASE_REG_ADDR(espi))       // 主模式进程2传输的数据[15:8]
#define MAS_THD2_IN_L(espi)               (0x28 + ESPI_BASE_REG_ADDR(espi))       // 主模式进程2传输的数据[7:0]
#define MAS_THD2_OUT_H(espi)              (0x2a + ESPI_BASE_REG_ADDR(espi))       // 主模式进程2接收的数据[15:8]
#define MAS_THD2_OUT_L(espi)              (0x2c + ESPI_BASE_REG_ADDR(espi))       // 主模式进程2接收的数据[7:0]
#define THD1_CRC_ERR_H(espi)              (0x2e + ESPI_BASE_REG_ADDR(espi))       // 主模式进程1的CRC错误计数[15:8]
#define THD1_CRC_ERR_L(espi)              (0x30 + ESPI_BASE_REG_ADDR(espi))       // 主模式进程1的CRC错误计数[7:0]
#define THD2_CRC_ERR_H(espi)              (0x32 + ESPI_BASE_REG_ADDR(espi))       // 主模式进程2的CRC错误计数[15:8]
#define THD2_CRC_ERR_L(espi)              (0x34 + ESPI_BASE_REG_ADDR(espi))       // 主模式进程2的CRC错误计数[7:0]
#define SLV_CRC_ERR_H(espi)               (0x36 + ESPI_BASE_REG_ADDR(espi))       // 从模式的CRC错误计数[15:8]
#define SLV_CRC_ERR_L(espi)               (0x38 + ESPI_BASE_REG_ADDR(espi))       // 从模式的CRC错误计数[7:0]
#define THD1_TIME_OUT_ERR_H(espi)         (0x3a + ESPI_BASE_REG_ADDR(espi))       // 主模式进程1传输超时错误统计[15:8]
#define THD1_TIME_OUT_ERR_L(espi)         (0x3c + ESPI_BASE_REG_ADDR(espi))       // 主模式进程1传输超时错误统计[7:0]
#define THD2_TIME_OUT_ERR_H(espi)         (0x3e + ESPI_BASE_REG_ADDR(espi))       // 主模式进程2传输超时错误统计[15:8]
#define THD2_TIME_OUT_ERR_L(espi)         (0x40 + ESPI_BASE_REG_ADDR(espi))       // 主模式进程2传输超时错误统计[7:0]
#define SLV_TIME_OUT_ERR_H(espi)          (0x42 + ESPI_BASE_REG_ADDR(espi))       // 从模式传输超时错误统计[15:8]
#define SLV_TIME_OUT_ERR_L(espi)          (0x44 + ESPI_BASE_REG_ADDR(espi))       // 从模式传输超时错误统计[7:0]
#define TIME_OUT_VALUE_3(espi)            (0x46 + ESPI_BASE_REG_ADDR(espi))       // 传输超时时间设置（必须大于268）[31:24]
#define TIME_OUT_VALUE_2(espi)            (0x48 + ESPI_BASE_REG_ADDR(espi))       // 传输超时时间设置（必须大于268）[23:16]
#define TIME_OUT_VALUE_1(espi)            (0x4a + ESPI_BASE_REG_ADDR(espi))       // 传输超时时间设置（必须大于268）[15:8]
#define TIME_OUT_VALUE_0(espi)            (0x4c + ESPI_BASE_REG_ADDR(espi))       // 传输超时时间设置（必须大于268）[7:0]
#define THD1_WR_ADDR(espi)                (0x4e + ESPI_BASE_REG_ADDR(espi))       // THD1 RAM 写地址
#define THD1_RD_ADDR(espi)                (0x50 + ESPI_BASE_REG_ADDR(espi))       // THD1 RAM 读地址
#define THD2_WR_ADDR(espi)                (0x52 + ESPI_BASE_REG_ADDR(espi))       // THD2 RAM 写地址
#define THD2_RD_ADDR(espi)                (0x54 + ESPI_BASE_REG_ADDR(espi))       // THD2 RAM 读地址

#else

#define ESPI_LOCK					(0x00)		  // 寄存器锁
#define ESPI_RESET                  (0x02)       // 复位控制
#define ESPI_MODE                   (0x04)       // 模式控制
#define ESPI_CRC_CLR                (0x06)       // CRC错误清零
#define ESPI_TIMEOUT_CLR            (0x08)       // 超时错误清零
#define THD1_RD_POS                 (0x0a)       // 控制读回数据为高8bit，还是低8bit
#define THD2_RD_POS                 (0x0c)       // 控制读回数据为高8bit，还是低8bit
#define THD1_STATUS                 (0x0e)       // 进程1相关状态显示
#define THD2_STATUS                 (0x10)       // 进程2相关状态显示
#define ESPI_STATE                  (0x12)       // ESPI 状态机
#define ESPI_OPCODE                 (0x14)       // 当前传输操作码
#define ESPI_ADDR_3                 (0x16)       // 当前传输地址[31:24]
#define ESPI_ADDR_2                 (0x18)       // 当前传输地址[23:16]
#define ESPI_ADDR_1                 (0x1a)       // 当前传输地址[15:8]
#define ESPI_ADDR_0                 (0x1c)       // 当前传输地址[7:0]
#define MAS_THD1_IN_H               (0x1e)       // 主模式进程1传输的数据[15:8]
#define MAS_THD1_IN_L               (0x20)       // 主模式进程1传输的数据[7:0]
#define MAS_THD1_OUT_H              (0x22)       // 主模式进程1接收的数据[15:8]
#define MAS_THD1_OUT_L              (0x24)       // 主模式进程1接收的数据[7:0]
#define MAS_THD2_IN_H               (0x26)       // 主模式进程2传输的数据[15:8]
#define MAS_THD2_IN_L               (0x28)       // 主模式进程2传输的数据[7:0]
#define MAS_THD2_OUT_H              (0x2a)       // 主模式进程2接收的数据[15:8]
#define MAS_THD2_OUT_L              (0x2c)       // 主模式进程2接收的数据[7:0]
#define THD1_CRC_ERR_H              (0x2e)       // 主模式进程1的CRC错误计数[15:8]
#define THD1_CRC_ERR_L              (0x30)       // 主模式进程1的CRC错误计数[7:0]
#define THD2_CRC_ERR_H              (0x32)       // 主模式进程2的CRC错误计数[15:8]
#define THD2_CRC_ERR_L              (0x34)       // 主模式进程2的CRC错误计数[7:0]
#define SLV_CRC_ERR_H               (0x36)       // 从模式的CRC错误计数[15:8]
#define SLV_CRC_ERR_L               (0x38)       // 从模式的CRC错误计数[7:0]
#define THD1_TIME_OUT_ERR_H         (0x3a)       // 主模式进程1传输超时错误统计[15:8]
#define THD1_TIME_OUT_ERR_L         (0x3c)       // 主模式进程1传输超时错误统计[7:0]
#define THD2_TIME_OUT_ERR_H         (0x3e)       // 主模式进程2传输超时错误统计[15:8]
#define THD2_TIME_OUT_ERR_L         (0x40)       // 主模式进程2传输超时错误统计[7:0]
#define SLV_TIME_OUT_ERR_H          (0x42)       // 从模式传输超时错误统计[15:8]
#define SLV_TIME_OUT_ERR_L          (0x44)       // 从模式传输超时错误统计[7:0]
#define TIME_OUT_VALUE_3            (0x46)       // 传输超时时间设置（必须大于268）[31:24]
#define TIME_OUT_VALUE_2            (0x48)       // 传输超时时间设置（必须大于268）[23:16]
#define TIME_OUT_VALUE_1            (0x4a)       // 传输超时时间设置（必须大于268）[15:8]
#define TIME_OUT_VALUE_0            (0x4c)       // 传输超时时间设置（必须大于268）[7:0]
#define THD1_WR_ADDR                (0x4e)       // THD1 RAM 写地址
#define THD1_RD_ADDR                (0x50)       // THD1 RAM 读地址
#define THD2_WR_ADDR                (0x52)       // THD2 RAM 写地址
#define THD2_RD_ADDR                (0x54)       // THD2 RAM 读地址

#endif



#endif
