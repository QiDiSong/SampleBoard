/**********************************************************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: 
 * Author     : w00449361
 * Create     : 
 **********************************************************************************************************************/
#include "mipi_fpga.h"
#include "mipi_reg_fpga.h"
#include "fpga_drv.h"
#include "memy.h"
#include <time.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern U32 g_mipiFpgaMapAddr[];
#define FPGA_MIPI_READ_REG(cs, addr) FPGA_ReadReg_Lego(cs, (g_mipiFpgaMapAddr[cs] + (addr)))
#define FPGA_MIPI_WRITE_REG(cs, addr, val) FPGA_WriteReg_Lego(cs, (g_mipiFpgaMapAddr[cs] + (addr)), val)


#define sysUsDelay(x) delay_mipi_us(x)

#define M_common_NULL_POINTER (-1)
#define FPGA_ESPI_R_WAITACK_TIMEUOT 1000

#define OS_EMBED_ASM   __asm__ __volatile__
#define INLINE         __attribute__ ((always_inline)) inline

#define MAX_SIZE_LINE_BYTE 4096

INLINE void delay_mipi_us(int cnt)
{    
    int i;
    for (i = 0; i < 1000* cnt; i++){
        OS_EMBED_ASM("nop");
    }
}

void Mipi_WriteReg(U32 ulChipIndex, U32 chann, U32 ulAddr, U32 ulVal)
{
	// //apb wr
	// FPGA_WriteReg(0,0x0000a402,0x3120);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_ADDR_H(chann), (ulAddr >> 16) & 0xFFFF);
	// FPGA_WriteReg(0,0x0000a404,0x0108);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_ADDR_L(chann), ulAddr & 0xFFFF);
	// FPGA_WriteReg(0,0x0000a406,0x1);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_ADDR_VLD(chann), 0x1);
	// FPGA_WriteReg(0,0x0000a406,0x0);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_ADDR_VLD(chann), 0x0);

	// //op
	// FPGA_WriteReg(0,0x0000a408,0x1);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_WR_RD_OP(chann), 0x1);

	// //data
	// FPGA_WriteReg(0,0x0000a40a,0x0000);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_W_DATA_H(chann), (ulVal >> 16) & 0xFFFF);
	// FPGA_WriteReg(0,0x0000a40c,0x0004);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_W_DATA_L(chann), ulVal & 0xFFFF);
	// FPGA_WriteReg(0,0x0000a40e,0x1);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_W_DATA_VLD(chann), 0x1);
	// FPGA_WriteReg(0,0x0000a40e,0x0);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_W_DATA_VLD(chann), 0x0);
	// //str
	// FPGA_WriteReg(0,0x0000a400,0x1);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_REQUEST(chann), 0x1);
	// FPGA_WriteReg(0,0x0000a400,0x0);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_REQUEST(chann), 0x0);

	
}
U32 Mipi_ReadReg(U32 ulChipIndex, U32 chann, U32 ulAddr)
{
	U32 val_high = 0x0;
	U32 val_low = 0x0;
	// //apb rd
	// //addr
	// FPGA_WriteReg(0,0x0000a402,0x3120);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_ADDR_H(chann), (ulAddr >> 16) & 0xFFFF);
	// FPGA_WriteReg(0,0x0000a404,0x0120);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_ADDR_L(chann), ulAddr & 0xFFFF);
	// FPGA_WriteReg(0,0x0000a406,0x1);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_ADDR_VLD(chann), 0x1);
	// FPGA_WriteReg(0,0x0000a406,0x0);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_APB_ADDR_VLD(chann), 0x0);

	// //op
	// FPGA_WriteReg(0,0x0000a408,0x0);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_WR_RD_OP(chann), 0x0);
	// //str
	// FPGA_WriteReg(0,0x0000a400,0x1);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_REQUEST(chann), 0x1);
	// FPGA_WriteReg(0,0x0000a400,0x0);
	FPGA_MIPI_WRITE_REG(ulChipIndex, CH_REQUEST(chann), 0x0);

	// FPGA_ReadReg(0,0x0000a414);
	FPGA_MIPI_READ_REG(ulChipIndex, CH_APB_R_DATA_VLD(chann));
	// FPGA_ReadReg(0,0x0000a410);
	val_high = FPGA_MIPI_READ_REG(ulChipIndex, CH_APB_R_DATA_H(chann));
	
	// FPGA_ReadReg(0,0x0000a412);
	val_low = FPGA_MIPI_READ_REG(ulChipIndex, CH_APB_R_DATA_L(chann));

	return((val_high << 16) + val_low);
}


void writeIQData(U32 chip, U32 chann, U16 addr, U16 ii, U16 qq)
{

    FPGA_MIPI_WRITE_REG(chip, MIPI_CH_RAM_ADDR(chann), addr);
    FPGA_MIPI_WRITE_REG(chip, MIPI_CH_RAM_DAT_I(chann), ii);
    FPGA_MIPI_WRITE_REG(chip, MIPI_CH_RAM_DAT_Q(chann), qq);
    // enable send
	FPGA_MIPI_WRITE_REG(chip, MIPI_CH_RAM_WEN(chann), 0x1);
	// disable
	FPGA_MIPI_WRITE_REG(chip, MIPI_CH_RAM_WEN(chann), 0x0);
}

// remove space at head of a string
int ltrim(char* str)
{
	if(str == NULL || *str == '\0')
	{
		return 0;
	}
	int len = 0;
	char* p = str;
	while(*p != 0 && isspace(*p))
	{
		++p;
		++len;
	}
	memmove(str, p, strlen(str) - len + 1);
	return 0;
}
int MipiParsingFileAndSend(char* configFile, U32 chann, U32 startAddr)
{
	U32 addrRpu = 0;
	FILE *conff = NULL;
	char strline[MAX_SIZE_LINE_BYTE];

    U32 dataiq = 0;

	if(configFile == NULL)
	{
		printf(" Illegal parameters, invaid config file !\n");
		return 0;
	}
	// test configure file exist
	if((access(configFile, F_OK)) ==-1 )
	{
		printf("Configure file not exist!\n");
		return 0;
	}
	// parse
	if((conff = fopen(configFile, "r")) == NULL)
	{
		printf("Configure file open failed!\n");
	}
	else
	{
		// process
        addrRpu = startAddr;
		while(!feof(conff))
		{
			strline[0] = '\0';
			if(fgets(strline, MAX_SIZE_LINE_BYTE, conff))
			{
				// skip comments
				ltrim( strline);
				if((strline[0] == '/' && strline[1] == '/') ||
						(strline[0] == '#') ||
						(strline[0] == '\n') ||
						(strlen(strline) < 1)) // empty line
				{
					continue;
				}
                dataiq = (U32)strtoul(strline, NULL, 16);
				(void)writeIQData(0, chann, addrRpu, (dataiq & 0xffff), (dataiq >> 16) & 0xffff);

			}
			addrRpu = addrRpu + 0x1;
		}
	}
	if(conff){
		fclose(conff);
		conff = NULL;
	}

	printf("WRITE SUCCESS.\n");
	return 0;
}

U32 g_debug_level = 2;

void CfgHeader(U32 mipi_ch, U32 chirp,
	U32 shortword0, U32 shortword1, U32 shortword2,U32 shortword3,U32 shortword4,U32 shortword5,U32 shortword6,U32 shortword7)
{

	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_ADDR(mipi_ch), chirp);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_DAT(mipi_ch, 7), shortword7);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_DAT(mipi_ch, 6), shortword6);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_DAT(mipi_ch, 5), shortword5);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_DAT(mipi_ch, 4), shortword4);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_DAT(mipi_ch, 3), shortword3);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_DAT(mipi_ch, 2), shortword2);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_DAT(mipi_ch, 1), shortword1);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_DAT(mipi_ch, 0), shortword0);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_WEN(mipi_ch),	0x1);
	FPGA_MIPI_WRITE_REG(0, MIPI_CH_HEAD_WEN(mipi_ch),	0x0);

}	

int MipiCfgHeader(U32 chann_cnt, U32 chirp_cnt, U32 chirp_length)
{
	HeaderRegisterType header = {0};
	int chann = 0;
	int chirp = 0;
	for( chann = 0; chann < chann_cnt; chann++){

		for(chirp = 0; chirp < chirp_cnt; chirp++){

			header.bits.chirp_data_start_offset = (chirp & 0x1) * chirp_length;
			header.bits.chirp_num = chirp_cnt - 1;
			header.bits.wave_entry_h = 0;
			header.bits.wave_entry_l = 0;
			header.bits.frame_type = 0;
			if(chirp_cnt == 1){
				header.bits.SOF_EOF = 0x3;
				header.bits.subframe_SOF_EOF = 0x3;
			}
			else{
				if(chirp == 0){
					header.bits.SOF_EOF = 0x1;
					header.bits.subframe_SOF_EOF = 0x1;
				}else if (chirp == (chirp_cnt - 1))
				{
					header.bits.SOF_EOF = 0x2;
					header.bits.subframe_SOF_EOF = 0x2;
				}else
				{
					header.bits.SOF_EOF = 0x0;
					header.bits.subframe_SOF_EOF = 0x0;
				}
			}
			header.bits.packet_num = chirp_cnt - 1;
			header.bits.subframe_type = 0;
			header.bits.rx_ultra_flag = 0;
			header.bits.dual_packet_en = 0;
			header.bits.dual_phy = 0x1;
			header.bits.chirp_type = 0x0;
			header.bits.rx_lane_enable  = 0xff;
			header.bits.d2a_rx_seg_length_0 = chirp_length -1;
			header.bits.d2a_rx_seg_length_1 = 0;

			if(g_debug_level > 1){
				printf("chann: %u, chirp:%u, header: %04x, %04x, %04x, %04x,	%04x, %04x, %04x, %04x. \n",
										chann,
										chirp, 
										header.u128_16.u16_0,
										header.u128_16.u16_1,
										header.u128_16.u16_2,
										header.u128_16.u16_3,
										header.u128_16.u16_4,
										header.u128_16.u16_5,
										header.u128_16.u16_6,
										header.u128_16.u16_7);
			}
			
			CfgHeader(chann, chirp, 
				header.u128_16.u16_0,
				header.u128_16.u16_1,
				header.u128_16.u16_2,
				header.u128_16.u16_3,
				header.u128_16.u16_4,
				header.u128_16.u16_5,
				header.u128_16.u16_6,
				header.u128_16.u16_7);

		}
	}
	return 0;
}
