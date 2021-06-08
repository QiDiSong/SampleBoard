#include <errno.h>
#include <memory.h>
// #include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "espi_fpga.h"
#include "espi_reg_fpga.h"
#include "mipi_reg_fpga.h"
#include "fpga_drv.h"
#include "memy.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define _MACROSTR(x) #x
#define MACROSTR(x) _MACROSTR(x)
#define BUILD_DATE MACROSTR(XUE_COMPILE_DATA)
#define BUILD_TIME MACROSTR(XUE_COMPILE_TIME)

extern void handle_udp_msg(int fd);
extern int newSocket(int *SockFd);
extern void handle_tcp_msg(int fd);
extern int newTCPSocket(int *SockFd);
extern void udpRecvThread(int *fd);
extern void test_time();

extern U32 g_espiFpgaMapAddr[];
extern int g_espiFdFpga[];
extern U32 g_mipiFpgaMapAddr[];
extern int g_mipiFdFpga[];

int main(int argc, char *argv[])
{

    for(int ii = 0; ii < ESPI_FPGA_MODULE_CNT; ii++){
        g_espiFdFpga[ii] = fpgaMemMap((U32)ESPI_BASE_REG_ADDR(ii), &g_espiFpgaMapAddr[ii]);
        if(g_espiFdFpga[ii] == -1){
            printf("Memmap error. Exit...\n");
            
            for(int jj = 0; jj < ii; jj ++){
                (void)fpgaMemUnmap(g_espiFdFpga[jj], g_espiFpgaMapAddr[jj]);
            }
 
            return -1;
        }
    }

    // test_time();
    int fd;
    // 防止THU1 reject 
    FPGA_EspiReset(0, 0);
    FPGA_EspiReset(1, 0);

    if(newTCPSocket(&fd))
    {
        printf("socket create fail\n");
        return -1;
    }
    handle_tcp_msg(fd);
    printf("Line:%d\n", __LINE__);
    close(fd);
    exit(0);
    while (1) {
        sleep(0);
    }
    
    for(int ii = 0; ii < ESPI_FPGA_MODULE_CNT; ii++){
        (void)fpgaMemUnmap(g_espiFdFpga[ii], g_espiFpgaMapAddr[ii]);
    }
	return 0;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
