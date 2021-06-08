/**********************************************************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: 
 * Author     : w00449361
 * Create     : 
 **********************************************************************************************************************/
#ifndef MIPI_DRV_FPGA_REG_H
#define MIPI_DRV_FPGA_REG_H

#include "fpga_drv.h"
#include "utils.h"

#define MIPI_MODULE_OFFSET (0x0a000)
#define MIPI_BASE_REG_ADDR(mipi) (FPGA_BASE_ADDR + MIPI_MODULE_OFFSET + (mipi)*0x1000)

#define MIPI_VERSION			                (0x0000)
#define REG_CLR			                        (0x0002)

#define CH_BW                                   (0x100)
#define MIPI_CH_HEAD_ADDR(chann)			    ((chann* CH_BW) + 0x0004)
#define MIPI_CH_HEAD_DAT(chann, head_num)	    ((chann* CH_BW) + 0x0006 + (0x2 * (head_num)))
#define MIPI_CH_HEAD_WEN(chann)			        ((chann* CH_BW) + 0x0016)
#define MIPI_CH_RAM_ADDR(chann)			        ((chann* CH_BW) + 0x0018)
#define MIPI_CH_RAM_DAT_I(chann)			    ((chann* CH_BW) + 0x001A)
#define MIPI_CH_RAM_DAT_Q(chann)			    ((chann* CH_BW) + 0x001C)
#define MIPI_CH_RAM_WEN(chann)			        ((chann* CH_BW) + 0x001E)
#define MIPI_CH_FRM_NUM(chann)			        ((chann* CH_BW) + 0x0020)
#define MIPI_CH_LOOP_NUM(chann)			        ((chann* CH_BW) + 0x0022)
#define MIPI_CH_PKG_NUM(chann)			        ((chann* CH_BW) + 0x0024)
#define MIPI_CH_SEND_EN(chann)			        ((chann* CH_BW) + 0x0026)
#define MIPI_CH_SEND_STOP(chann)			    ((chann* CH_BW) + 0x0028)
#define MIPI_CH_SEND_CNT(chann)			        ((chann* CH_BW) + 0x002A)
#define MIPI_CH_SEND_OV(chann)			        ((chann* CH_BW) + 0x002C)
#define MIPI_CH_FRM_NUM_CUR(chann)			    ((chann* CH_BW) + 0x002E)
#define MIPI_CH_LOOP_NUM_CUR(chann)			    ((chann* CH_BW) + 0x0030)
#define MIPI_CH_PKG_NUM_CUR(chann)			    ((chann* CH_BW) + 0x0032)
#define MIPI_CH_FRM_SEND_OV(chann)			    ((chann* CH_BW) + 0x0034)
#define MIPI_CH_LOOP_SEND_OV(chann)		    	((chann* CH_BW) + 0x0036)
#define MIPI_CH_PKG_SEND_OV(chann)			    ((chann* CH_BW) + 0x0038)
#define MIPI_CH_ENABLE(chann)			        ((chann* CH_BW) + 0x003A)
#define CH_HFIFO_WR_ERR_CNT(chann)			    ((chann* CH_BW) + 0x003C)
#define CH_HFIFO_RD_ERR_CNT(chann)			    ((chann* CH_BW) + 0x003E)
#define CH_FIFO_HEAD_WR_CNT(chann)			    ((chann* CH_BW) + 0x0040)
#define MIPI_CH_RAM_WR_LANE0_CNT(chann)			((chann* CH_BW) + 0x0042)
#define MIPI_CH_RAM_WR_LANE1_CNT(chann)			((chann* CH_BW) + 0x0044)
#define MIPI_CH_RAM_WR_LANE2_CNT(chann)			((chann* CH_BW) + 0x0046)
#define MIPI_CH_RAM_WR_LANE3_CNT(chann)			((chann* CH_BW) + 0x0048)
#define MIPI_CH_RAM_WR_LANE4_CNT(chann)			((chann* CH_BW) + 0x004A)
#define MIPI_CH_RAM_WR_LANE5_CNT(chann)			((chann* CH_BW) + 0x004C)
#define MIPI_CH_RAM_WR_LANE6_CNT(chann)			((chann* CH_BW) + 0x004E)
#define MIPI_CH_RAM_WR_LANE7_CNT(chann)			((chann* CH_BW) + 0x0050)


#define CH_REQUEST(chann)			            ((chann* CH_BW) + 0x0400)
#define CH_APB_ADDR_H(chann)		            ((chann* CH_BW) + 0x0402)
#define CH_APB_ADDR_L(chann)		            ((chann* CH_BW) + 0x0404)
#define CH_APB_ADDR_VLD(chann)		            ((chann* CH_BW) + 0x0406)
#define CH_WR_RD_OP(chann)			            ((chann* CH_BW) + 0x0408)
#define CH_APB_W_DATA_H(chann)		            ((chann* CH_BW) + 0x040a)
#define CH_APB_W_DATA_L(chann)		            ((chann* CH_BW) + 0x040c)
#define CH_APB_W_DATA_VLD(chann)	            ((chann* CH_BW) + 0x040e)
#define CH_APB_R_DATA_H(chann)		            ((chann* CH_BW) + 0x0410)
#define CH_APB_R_DATA_L(chann)		            ((chann* CH_BW) + 0x0412)
#define CH_APB_R_DATA_VLD(chann)	            ((chann* CH_BW) + 0x0414)
#define CH_APB_READY(chann)			            ((chann* CH_BW) + 0x0416)
#define CH_APB_INTER_TOP_STATE(chann)           ((chann* CH_BW) + 0x0418)
#define CH_APB_ERR_CNT(chann)		            ((chann* CH_BW) + 0x041a)
#define CH_APB_WR_CNT(chann)		            ((chann* CH_BW) + 0x041c)
#define CH_APB_RD_CNT(chann)		            ((chann* CH_BW) + 0x041e)
#define CH_APB_W_R_ED_CNT(chann)	            ((chann* CH_BW) + 0x0420)
		                                        
#define CH_AXI_M_OP_STR(chann)		            ((chann* CH_BW) + 0x0440)
#define CH_AXI_M_WADDR_MPI(chann)	            ((chann* CH_BW) + 0x0442)
#define CH_AXI_M_WADDR_MPI_VLD(chann)           ((chann* CH_BW) + 0x0444)
#define CH_AXI_M_WDAT_MPI_H(chann)	            ((chann* CH_BW) + 0x0446)
#define CH_AXI_M_WDAT_MPI_L(chann)	            ((chann* CH_BW) + 0x0448)
#define CH_AXI_M_WDAT_MPI_VLD(chann)            ((chann* CH_BW) + 0x044a)
#define CH_AXI_M_RADDR_MPI(chann)	            ((chann* CH_BW) + 0x044c)
#define CH_AXI_M_RADDR_MPI_VLD(chann)           ((chann* CH_BW) + 0x044e)
#define CH_AXI_M_OP_MPI(chann)		            ((chann* CH_BW) + 0x0450)
#define CH_AXI_M_OP_MPI_VLD(chann)	            ((chann* CH_BW) + 0x0452)
#define CH_AXI_M_RDAT_MPI_H(chann)	            ((chann* CH_BW) + 0x0454)
#define CH_AXI_M_RDAT_MPI_L(chann)	            ((chann* CH_BW) + 0x0456)
#define CH_AXI_M_RDAT_MPI_VLD(chann)            ((chann* CH_BW) + 0x0458)
#define CH_AXI_MT_LITE_STATE(chann)	            ((chann* CH_BW) + 0x045a)
#define CH_AXI_M_WR_CNT(chann)		            ((chann* CH_BW) + 0x045c)
#define CH_AXI_M_RD_CNT(chann)		            ((chann* CH_BW) + 0x046e)
#define CH_AXI_M_TIMEOUT_CNT(chann)	            ((chann* CH_BW) + 0x0460)


#define CH0_PLL_LOCK_OUT(chann)						((chann* CH_BW) + 0x0822)
#define CH0_MMCM_LOCK_OUT(chann)                    ((chann* CH_BW) + 0x0824)
#define CH0_SYSTEM_RST_OUT(chann)                   ((chann* CH_BW) + 0x0826)
#define CH0_INIT_DONE(chann)                        ((chann* CH_BW) + 0x0828)
#define MIPI_CH0_DAT_LEN_DB(chann)                  ((chann* CH_BW) + 0x082a)
#define MIPI_CH0_DAT_LANE0_CNT(chann)               ((chann* CH_BW) + 0x082c)
#define MIPI_CH0_DAT_LANE1_CNT(chann)               ((chann* CH_BW) + 0x082e)
#define MIPI_CH0_DAT_LANE2_CNT(chann)               ((chann* CH_BW) + 0x0830)
#define MIPI_CH0_DAT_LANE3_CNT(chann)               ((chann* CH_BW) + 0x0832)
#define MIPI_CH0_DAT_LANE4_CNT(chann)               ((chann* CH_BW) + 0x0834)
#define MIPI_CH0_DAT_LANE5_CNT(chann)               ((chann* CH_BW) + 0x0836)
#define MIPI_CH0_DAT_LANE6_CNT(chann)               ((chann* CH_BW) + 0x0838)
#define MIPI_CH0_DAT_LANE7_CNT(chann)               ((chann* CH_BW) + 0x083a)
#define MIPI_CH0_WR_ERR_LANE0_CNT(chann)            ((chann* CH_BW) + 0x083c)
#define MIPI_CH0_WR_ERR_LANE1_CNT(chann)            ((chann* CH_BW) + 0x083e)
#define MIPI_CH0_WR_ERR_LANE2_CNT(chann)            ((chann* CH_BW) + 0x0840)
#define MIPI_CH0_WR_ERR_LANE3_CNT(chann)            ((chann* CH_BW) + 0x0842)
#define MIPI_CH0_WR_ERR_LANE4_CNT(chann)            ((chann* CH_BW) + 0x0844)
#define MIPI_CH0_WR_ERR_LANE5_CNT(chann)            ((chann* CH_BW) + 0x0846)
#define MIPI_CH0_WR_ERR_LANE6_CNT(chann)            ((chann* CH_BW) + 0x0848)
#define MIPI_CH0_WR_ERR_LANE7_CNT(chann)            ((chann* CH_BW) + 0x084a)
#define MIPI_CH0_RD_ERR_LANE0_CNT(chann)            ((chann* CH_BW) + 0x084c)
#define MIPI_CH0_RD_ERR_LANE1_CNT(chann)            ((chann* CH_BW) + 0x084e)
#define MIPI_CH0_RD_ERR_LANE2_CNT(chann)            ((chann* CH_BW) + 0x0850)
#define MIPI_CH0_RD_ERR_LANE3_CNT(chann)            ((chann* CH_BW) + 0x0852)
#define MIPI_CH0_RD_ERR_LANE4_CNT(chann)            ((chann* CH_BW) + 0x0854)
#define MIPI_CH0_RD_ERR_LANE5_CNT(chann)            ((chann* CH_BW) + 0x0856)
#define MIPI_CH0_RD_ERR_LANE6_CNT(chann)            ((chann* CH_BW) + 0x0858)
#define MIPI_CH0_RD_ERR_LANE7_CNT(chann)            ((chann* CH_BW) + 0x085a)
#define CH0_TXREADYHS(chann)                        ((chann* CH_BW) + 0x085c)
#define CH0_TXREADYESC(chann)                       ((chann* CH_BW) + 0x085e)
#define CH0_CL_STOPSTATE(chann)                     ((chann* CH_BW) + 0x0860)
#define CH0_CL_ENABLE(chann)                        ((chann* CH_BW) + 0x0862)
#define CH0_DL0_FORCETXSTOPMODE(chann)              ((chann* CH_BW) + 0x0864)
#define CH0_DL0_STOPSTATE(chann)                    ((chann* CH_BW) + 0x0866)
#define CH0_DL0_ENABLE(chann)                       ((chann* CH_BW) + 0x0868)


#define MIPI_ALLCH_SEND_EN			                (0x0C00)
#define MIPI_ALLCH_SEND_STOP		                (0x0C02)
#define MIPI_RST_CFG			                    (0x0C04)





typedef union {
    /* Define the struct bits  */
    struct {
        U32 chirp_data_start_offset		: 16;
        U32 reserve1					: 16;
 
        U32 reserve2					: 5;   
        U32 chirp_num					: 16;  
        U32 wave_entry_h				: 11;  
 
        U32 wave_entry_l				: 2;   
        U32 frame_type					: 1;   
        U32 SOF_EOF						: 2;   
        U32 packet_num					: 16;   
        U32 subframe_type				: 5;   
        U32 rx_ultra_flag				: 1;   
        U32 subframe_SOF_EOF			: 2;   
        U32 dual_packet_en				: 1;   
        U32 dual_phy					: 1;   
        U32 chirp_type					: 1;   
                                         
        U32 rx_lane_enable				: 8;   
        U32 d2a_rx_seg_length_0			: 12;  
        U32 d2a_rx_seg_length_1			: 12;  

    } bits;

    /* Define an unsigned member */
    struct {
        U16    u16_7;
        U16    u16_6;
        U16    u16_5;
        U16    u16_4;
        U16    u16_3;
        U16    u16_2;
        U16    u16_1;
        U16    u16_0;
    } u128_16;

} HeaderRegisterType;


#endif

