#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>


#include "utils.h"
#include "espi_fpga.h"
#include "mmic_fifo.h"

#define FIFO_1380_BASE_ADDR                   (0x03fb0000)          // SMC
// #define FIFO_1380_BASE_ADDR                   (0xff000000)            // DDR
#define DATA_WRITE_START_ADDR_RPU             (FIFO_1380_BASE_ADDR)
#define DATA_READ_START_ADDR_RPU              (DATA_WRITE_START_ADDR_RPU + 0x3000)
#define DATA_READ_SERIES_START_ADDR_RPU       (DATA_READ_START_ADDR_RPU + 0x3000)

#define DATA_WRITE_QUEUE_FRONT_ADDR_RPU       (FIFO_1380_BASE_ADDR + 0x4ff00)       // 0x03fb0000  + 0x4ff00  == 0x3FFFF00       0xff04ff00 
#define DATA_WRITE_QUEUE_REAR_ADDR_RPU        (FIFO_1380_BASE_ADDR + 0x4ff04)

#define DATA_READ_QUEUE_FRONT_ADDR_RPU       (FIFO_1380_BASE_ADDR + 0x4ff08)
#define DATA_READ_QUEUE_REAR_ADDR_RPU        (FIFO_1380_BASE_ADDR + 0x4ff0c)



#define SERVER_PORT 8888
#define MAX_DATA_ONCE (128)


// #define printf(para, ...)
// #define printf(para, ...)
typedef unsigned char uint8;

typedef struct {
    uint32 chipId;
    uint32 address;
    uint32 value;
} spiCmdSt;

#define OS_EMBED_ASM   __asm__ __volatile__
#define INLINE         __attribute__ ((always_inline)) inline
INLINE void my_delay_us(int cnt)
{
    int i;
    for (i = 0; i < 1000* cnt; i++){
        OS_EMBED_ASM("nop");
    }
}

int newTCPSocket(int *SockFd)
{
    *SockFd = socket(AF_INET,SOCK_STREAM,0);

    if (*SockFd < 0)
    {
        printf("TCP socket create error!\n");
        return -1;
    }

    struct sockaddr_in Server;
    memset(&Server, 0, sizeof(Server));
    Server.sin_family = AF_INET;
    Server.sin_addr.s_addr = INADDR_ANY;
    Server.sin_port = htons(SERVER_PORT);

    if(bind(*SockFd, (struct sockaddr *)&Server, sizeof(struct sockaddr)) == -1)
    {
        printf("TCP bind error!\n");
        return -1;
    }
    printf("bind success, fd%d\n", *SockFd);
    return 0;
}

int newSocket(int *SockFd)
{
    *SockFd = socket(AF_INET,SOCK_DGRAM,0);
    if (*SockFd < 0)
    {
        printf("socket create fail!\n");
        return -1;
    }
    struct sockaddr_in Server;
    memset(&Server, 0, sizeof(Server));
    Server.sin_family = AF_INET;
    Server.sin_addr.s_addr = htonl(INADDR_ANY);
    Server.sin_port = htons(SERVER_PORT);
    if(bind(*SockFd, (struct sockaddr *)&Server, sizeof(struct sockaddr)) == -1)
    {
        printf("socket bind failed!\n");
        return -1;
    }
    printf("bind success, fd%d\n", *SockFd);
    return 0;
}

void test_time()
{
    int i ;
    int buf[12] = {1, 2, 3};
    int SendCmdopcode = 0x52;
    clock_t start, end, start_once, end_once;
    start = clock();
    for(i = 0; i < 100; i++)
    {
        start_once = clock(); 
        // U32 write_rear = FPGA_Espi_ReadReg(0, DATA_WRITE_QUEUE_iREAR_ADDR_RPU, 0);
        // FPGA_Espi_Burst_WriteReg(0, (DATA_WRITE_START_ADDR_RPU), buf, SendCmdopcode, 0);
        (void)FPGA_Espi_WriteReg(0, DATA_WRITE_QUEUE_REAR_ADDR_RPU, i, 0);
        end_once = clock();
        printf("once_time:%fms\n", (double)(end_once - start_once)/ 20);
    }
    end = clock();
    printf("total time:%fSec\n", (double)(end - start) / 1000 / 20);
}

void handle_tcp_msg(int fd)
{
    char buf[12];
    socklen_t len;
    int count;
    struct sockaddr_in client_addr;         //client_addr用于记录发送方的地址信息
    printf("Begin fd:%d\n", __LINE__);
    len = sizeof(client_addr);
    listen(fd, 5);
    printf("Line:%d\n", __LINE__);
    int client_accept = accept(fd, (struct sockaddr *)&client_addr, &len);
    int cmd_times = 0;
    int series_times = 0;
    while(1)
    {
        memset(buf, 0, 12);
        count = recv(client_accept, buf, 12, 0);
        cmd_times++;
        int DATA_READ_SERIES_START_ADDR_RPU_EachTcpPack = DATA_READ_SERIES_START_ADDR_RPU;
        if(count == -1)
        {
            printf("master PC send nothing!\n");
            return;
        }
        if(count > 0)
        {
            int chipId = *(uint32*)(buf);
            int address = *(uint32*)(buf + 4);
            int value = *(uint32*)(buf + 8);
            uint8 CpuId = (uint8)((chipId & 0x3) / 2);
            // printf("ChipID:%08x   ", chipId);
            // printf("Address:%08x   ", address);
            // printf("Value:%08x\n", value);
            // printf("MMICID:%d\n", CpuId);
            U32 write_rear = FPGA_Espi_ReadReg(CpuId, DATA_WRITE_QUEUE_REAR_ADDR_RPU, 0);
            U32 write_front = FPGA_Espi_ReadReg(CpuId, DATA_WRITE_QUEUE_FRONT_ADDR_RPU, 0);

            if((chipId & 0x4) == 0x04)
            {
                // write
                int SendCmdopcode = 0x52;
                int send_cmd_times = 0;
                while(1)
                {
                    if(IsQueueFull(write_front, write_rear) == 1){
                        send_cmd_times++;
                        if(send_cmd_times == 10000)
                        {
                            printf("Write FIFO is full!\n");
                            return -1;      // -1 表示写FIFO是满的
                        }
                        // sleep(0);
                        continue;
                    }
                    // send to RPU
                    // int reta = FPGA_Espi_Burst_WriteReg(CpuId, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear), buf, SendCmdopcode, 0);
                    FPGA_Espi_WriteReg(CpuId, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear), chipId, 0);
                    FPGA_Espi_WriteReg(CpuId, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear + 4), address, 0);
                    FPGA_Espi_WriteReg(CpuId, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear + 8), value, 0);
                    // update write rear
                    (void)FPGA_Espi_WriteReg(CpuId, DATA_WRITE_QUEUE_REAR_ADDR_RPU, (write_rear + 1) % PACKET_CNT_QUEUE, 0);
                    break;
                }
            }
            // read
            else
            {
                // 发送read命令
                int send_cmd_times = 0;
                int SendCmdopcode = 0x52;
                while(1)
                {
                    if(IsQueueFull(write_front, write_rear) == 1){
                        send_cmd_times++;
                        if(send_cmd_times == 10000)
                        {
                            printf("Write FIFO is full!\n");
                            return -1;      // -1 表示写FIFO是满的
                        }
                        // sleep(0);
                        continue;
                    }
                    FPGA_Espi_WriteReg(CpuId, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear), chipId, 0);
                    FPGA_Espi_WriteReg(CpuId, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear + 4), address, 0);
                    FPGA_Espi_WriteReg(CpuId, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear + 8), value, 0);
                    // FPGA_Espi_Burst_WriteReg(CpuId, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear), buf, SendCmdopcode, 0);
                    FPGA_Espi_WriteReg(CpuId, DATA_WRITE_QUEUE_REAR_ADDR_RPU, (write_rear + 1) % PACKET_CNT_QUEUE, 0);
                    break;
                }
                // 等待从RPU上读消息
                int readtimes = 0;
                U32 read_rear = 0, read_front = 0;
                while(1)
                {
                    
                    read_rear = FPGA_Espi_ReadReg(CpuId, DATA_READ_QUEUE_REAR_ADDR_RPU, 0);
                    read_front = FPGA_Espi_ReadReg(CpuId, DATA_READ_QUEUE_FRONT_ADDR_RPU, 0);
                    if (read_rear == read_front)
                    {
                        readtimes++;
                        // 判断是否从RPU上读取到数据
                        continue;
                    }
                    readtimes = 0;
                    uint32 ReadRegTmp[3];
                    ReadRegTmp[0] = FPGA_Espi_ReadReg(CpuId, (DATA_READ_START_ADDR_RPU + sizeof(spiCmdSt) * read_front), 0);
                    ReadRegTmp[1] = FPGA_Espi_ReadReg(CpuId, (4 + DATA_READ_START_ADDR_RPU + sizeof(spiCmdSt) * read_front), 0);
                    ReadRegTmp[2] = FPGA_Espi_ReadReg(CpuId, (8 + DATA_READ_START_ADDR_RPU + sizeof(spiCmdSt) * read_front), 0);
                    // update read front
                    (void)FPGA_Espi_WriteReg(CpuId, DATA_READ_QUEUE_FRONT_ADDR_RPU, (read_front + 1) % PACKET_CNT_QUEUE, 0);

                    int send_size = 0;
                    int loop_time = 0;
                    if((chipId & 0x10) == 0x10)
                    {
                        series_times++;
                        if(ReadRegTmp[1] == 0xffffffff)
                        {
                            printf("Something error occurs in RPU!\n");
                            return -1;
                        }
                        U8 seriesBuff[MAX_DATA_ONCE] = {0};
                        U8 totalData[MAX_DATA_ONCE * 10] = {0};
                        int readSeriesOpCode = 0xff;
                        loop_time = (value / MAX_DATA_ONCE) + (((value % MAX_DATA_ONCE) == 0 ) ? 0 : 1);
                        for(int idx = 0; idx < loop_time; idx++)
                        {
                            send_size += MAX_DATA_ONCE;
                            FPGA_Espi_Burst_ReadReg(CpuId, DATA_READ_SERIES_START_ADDR_RPU_EachTcpPack, readSeriesOpCode, seriesBuff, 0);
                            memcpy(&totalData[idx * MAX_DATA_ONCE], seriesBuff, MAX_DATA_ONCE);
                            memset(seriesBuff, 0, sizeof(seriesBuff));                                   // reset to 0
                            DATA_READ_SERIES_START_ADDR_RPU_EachTcpPack += MAX_DATA_ONCE;                // 每个TCP包都是从23000一直往后读，下个TCP包重新冲230000开始读
                        }
                        send(client_accept, totalData, send_size, 0);
                        // printf("series_times: %d  send %d Bytes to master computer!\n", series_times, send_size);
                        send_size = 0;
                        break;
                    }
                    send(client_accept, &ReadRegTmp[2], sizeof(ReadRegTmp[2]), 0);
                    // printf("send read ret value success!\n");
                    break;                    
                }
            }
        }
        // sleep(0);
    }
}










// void handle_udp_msg(int fd)
// {
//     char buf[12];
//     socklen_t len;
//     int count;
//     struct sockaddr_in client_addr;  //client_addr用于记录发送方的地址信息
//     printf("Begin fd:%d\n", __LINE__);
//     while(1)
//     {
//         memset(buf, 0, 12);
//         len = sizeof(client_addr);
//         count = recvfrom(fd, buf, 12, 0, (struct sockaddr*)&client_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
//         cnt++;
//         if(count == -1)
//         {
//             printf("recieve data fail!\n");
//             return;
//         }

//         printf("cnt: %d\t", cnt);
//         // printf("length: %d\n", count);
//         int chipId = *(uint32*)(buf);
//         int address = *(uint32*)(buf + 4);
//         int value = *(uint32*)(buf + 8);
//         // printf("ChipID:%08x   ", chipId);
//         // printf("Address:%08x   ", address);
//         // printf("Value:%08x   ", value);
//         // printf("\n\n");

//         U32 write_rear = FPGA_Espi_ReadReg(0, DATA_WRITE_QUEUE_REAR_ADDR_RPU, 0);
//         U32 write_front = FPGA_Espi_ReadReg(0, DATA_WRITE_QUEUE_FRONT_ADDR_RPU, 0);
//         // printf("write_rear:%d\n", write_rear);
//         // printf("write_front:%d\n", write_front);
//         if((chipId & 0x4) == 0x04)
//         {
//             // write
//             int SendCmdopcode = 0x52;
//             int send_cmd_times = 0;
//             while(1)
//             {
//                 if(IsQueueFull(write_front, write_rear) == 1){
//                     send_cmd_times++;
//                     if(send_cmd_times == 10000)
//                     {
//                         printf("Write FIFO is full!\n");
//                         return -1;      // -1 表示写FIFO是满的
//                     }
//                     sleep(0); 
//                     continue;
//                 }
//                 // send to RPU
//                 FPGA_Espi_Burst_WriteReg(0, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear), buf, SendCmdopcode, 0);
//                 // update write rear
//                 (void)FPGA_Espi_WriteReg(0, DATA_WRITE_QUEUE_REAR_ADDR_RPU, (write_rear + 1) % PACKET_CNT_QUEUE, 0);
//                 break;
//             }

//             sendto(fd, &address, sizeof(address), 0, (struct sockaddr*)&client_addr, len);
//         }
//         // read
//         else
//         {
//             // 发送read命令
//             int send_cmd_times = 0;
//             int SendCmdopcode = 0x52;               // 0xD2 opcode，burst_width:10, 长度单位为字， burst_length = 0001,每次burst传输3单位数据
//             while(1)
//             {
//                 if(IsQueueFull(write_front, write_rear) == 1){
//                     send_cmd_times++;
//                     if(send_cmd_times == 10000)
//                     {
//                         printf("Write FIFO is full!\n");
//                         return -1;      // -1 表示写FIFO是满的
//                     }
//                     sleep(0);
//                     continue;
//                 }
//                 FPGA_Espi_Burst_WriteReg(0, (DATA_WRITE_START_ADDR_RPU + sizeof(spiCmdSt) * write_rear), buf, SendCmdopcode, 0);
//                 (void)FPGA_Espi_WriteReg(0, DATA_WRITE_QUEUE_REAR_ADDR_RPU, (write_rear + 1) % PACKET_CNT_QUEUE, 0);
//                 break;
//             }

//             // 等待从RPU上读消息
//             while(1)
//             {
//                 U32 read_rear = FPGA_Espi_ReadReg(0, DATA_READ_QUEUE_REAR_ADDR_RPU, 0);
//                 U32 read_front = FPGA_Espi_ReadReg(0, DATA_READ_QUEUE_FRONT_ADDR_RPU, 0);
//                 if (read_rear == read_front)
//                 {
//                     // 判断是否从RPU上读取到数据
//                     sleep(0);
//                     continue;
//                 }
//                 uint32 ReadRegTmp[3];
//                 // SendCmdopcode = 0xD2;        // FPGA_Espi_Burst_ReadReg可能有问题，使用最基本的readreg函数来读取
//                 // FPGA_Espi_Burst_ReadReg(0, (DATA_READ_START_ADDR_RPU + sizeof(spiCmdSt) * read_front), SendCmdopcode, ReadRegTmp, 0);
//                 ReadRegTmp[0] = FPGA_Espi_ReadReg(0, (DATA_READ_START_ADDR_RPU + sizeof(spiCmdSt) * read_front), 0);
//                 ReadRegTmp[1] = FPGA_Espi_ReadReg(0, (4 + DATA_READ_START_ADDR_RPU + sizeof(spiCmdSt) * read_front), 0);
//                 ReadRegTmp[2] = FPGA_Espi_ReadReg(0, (8 + DATA_READ_START_ADDR_RPU + sizeof(spiCmdSt) * read_front), 0);
//                 // update read front
//                 (void)FPGA_Espi_WriteReg(0, DATA_READ_QUEUE_FRONT_ADDR_RPU, (read_front + 1) % PACKET_CNT_QUEUE, 0);
//                 // 将ReadRegTmp[2]（即RPU返回值）发送给上位机，用于同步
//                 // printf("ReadRegTmp: %08x\t", ReadRegTmp[0]);
//                 // printf("ReadRegTmp_address: %08x\t", ReadRegTmp[1]);
//                 // printf("ReadRegTmp_value: %08x\n\n", ReadRegTmp[2]);
//                 sendto(fd, &ReadRegTmp[2], sizeof(ReadRegTmp[2]), 0, (struct sockaddr*)&client_addr, len);
//                 break;
//             }
//         }
//         sleep(0);
//     }
//     printf("Line:%d\n", __LINE__);
// }
