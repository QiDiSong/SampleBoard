
#include "memy.h"
#include "fpga_drv.h"

U32 g_espiFpgaMapAddr[ESPI_FPGA_MODULE_CNT] = {0};
int g_espiFdFpga[ESPI_FPGA_MODULE_CNT] = {0};

U32 g_mipiFpgaMapAddr[MIPI_FPGA_MODULE_CNT] = {0};
int g_mipiFdFpga[MIPI_FPGA_MODULE_CNT] = {0};

U16 FPGA_ReadReg(U32 ulChipIndex, U32 ulAddr)
{
    U16 usValue;
	
    usValue = FPGA_ReadReg_Lego(ulChipIndex, ulAddr);

    return usValue;
}
void FPGA_WriteReg(U32 ulChipIndex, U32 ulAddr, U16 usData)
{
    U16 usValue;
    FPGA_WriteReg_Lego(ulChipIndex, ulAddr, usData);
    return;
}


U16 FPGA_ReadReg_Lego(U32 ulChipIndex, U32 ulAddr)
{
    U16 usValue = 0;
    if ((0 != ulChipIndex)) {
        printf("[FPGA_RegRead] ERROR ulChipIndex is (%d) ulAddr is (0x%x).\r\n", ulChipIndex, ulAddr, 0, 0);
        return (U16)ERROR;
    }
    usValue = *(volatile U16*)(ulAddr);


    usValue = BYTE_SWAP16(usValue);

    return usValue;
}
void FPGA_WriteReg_Lego(U32 ulChipIndex, U32 ulAddr, U16 usData)
{
    U16 usValue;
	
    if ((0 != ulChipIndex)) {
        printf("[FPGA_RegWrite] ERROR ulChipIndex is (%d) ulAddr is (0x%x).\r\n", ulChipIndex, ulAddr, 0, 0);
        return;
    }
	
    usValue = BYTE_SWAP16(usData);
    *(volatile U16 *)(ulAddr) = usValue;

    return;
}


