
#include "memy.h"
#include <stdio.h>
#include <time.h>
#define ADDR_BASE_PAGE_ALIGNED(addr) (addr < PAGE_SIZE ? 0: (addr - addr % PAGE_SIZE))
#define ADDR_OFFSET_PAGE_ALINGED(addr) (addr % PAGE_SIZE)

enum ErrNo
{
	MemOpenError = 1,
	MemMapError = 2,
	WrongRegAddr = 3
};

#define MMAP_ADDR_ASSERT(addr) \
	do {\
		if ( !(addr >= 0x68000000 && addr <= 0x9fffffff) ) {\
			printf("illegal mmap requested address. addr should be within [0x68000000, 0x9fffffff].\n"); \
			return -1; \
        }\
		if (addr % 2 != 0) {\
			printf("illegal mmap requested address. addr should be 2byte aligned.\n"); \
			return -1; \
        }\
    }\
    while (0)


// struct timespec g_timeLastTxTmp31 = {0,0};
// struct timespec g_timeLastTxTmp32 = {0,0};
// struct timespec g_timeLastTxTmp33 = {0,0};
// struct timespec g_timeLastTxTmp34 = {0,0};



//int _MemMap(int dev_fd, U8* map_base, unsigned int map_mem_base);
void _MemUnmap(int dev_fd, U8* map_base);

/* _MemMap and _MemUnmap should be match
 * */
int _MemMap(int *dev_fd, U8** map_base, U32 map_mem_base)
{
	//int dev_fd = 0;
	//U8 * map_base = NULL;
	*dev_fd = open(MEM_DEV, O_RDWR | O_SYNC);
	if(*dev_fd < 0)
	{
		//printf("open /dev/mem error. \n");
		return MemOpenError; 
	}
	*map_base = (U8*)mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, *dev_fd, map_mem_base);
	if(MAP_FAILED == *map_base)
	{
		//printf("mmap error. \n");
		return MemMapError; 
	}
	return 0;
}
/*
 * _MemMap and _MemUnmap should be match
 * */
void _MemUnmap(int dev_fd, U8* map_base)
{
	if(dev_fd)
	{
		close(dev_fd);
	}
	munmap(map_base, MAP_SIZE);
	dev_fd = 0;
}


// caller should check parameters
int mWriteRegAbsoluteAddr(U32 addr, U16 value)
{

    //clock_gettime(CLOCK_REALTIME, &g_timeLastTxTmp31);
	MMAP_ADDR_ASSERT(addr);
	int dev_fd = 0;
	U8 * map_base = NULL;
    //U32 offset = 0;
	int ret = 0;

	ret = _MemMap(&dev_fd, &map_base, ADDR_BASE_PAGE_ALIGNED(addr));
	if(ret)
		return ret;
	//clock_gettime(CLOCK_REALTIME, &g_timeLastTxTmp32);
	*(volatile U16 *)(map_base + ADDR_OFFSET_PAGE_ALINGED(addr)) = value;
	//printf("write %x, %x \n", map_base + ADDR_OFFSET_PAGE_ALINGED(addr), value);
	//clock_gettime(CLOCK_REALTIME, &g_timeLastTxTmp33);
	_MemUnmap(dev_fd, map_base);
	//clock_gettime(CLOCK_REALTIME, &g_timeLastTxTmp34);
	//usleep(100);
	return ret;
}
// caller should check parameters
int mReadRegAbsoluteAddr(U16 * value, U32 addr)
{
	MMAP_ADDR_ASSERT(addr);
	int dev_fd = 0;
	U8 * map_base = NULL;
   // U32 offset = 0;
	int ret = 0;
	//printf("mReadRegAbsoluteAddr %x, %x, %x \n", addr, ADDR_BASE_PAGE_ALIGNED(addr), ADDR_OFFSET_PAGE_ALINGED(addr));
	ret = _MemMap(&dev_fd, &map_base, ADDR_BASE_PAGE_ALIGNED(addr));
	if(ret)
		return ret;
	//printf("map_base %x\n", map_base);
	*value = *(volatile U16 *)(map_base + ADDR_OFFSET_PAGE_ALINGED(addr));
	//printf("read %x, %x \n", map_base + ADDR_OFFSET_PAGE_ALINGED(addr), *value);
	//printf("map_base1 %x, %x \n", map_base + ADDR_OFFSET_PAGE_ALINGED(addr), *value);
	//printf("map_base1 %x, %x \n", map_base + ADDR_OFFSET_PAGE_ALINGED(addr), *value);
	//printf("map_base1 %x, %x \n", map_base + ADDR_OFFSET_PAGE_ALINGED(addr), *value);
	_MemUnmap(dev_fd, map_base);

	return ret;
}


int fpgaMemMap(U32 addrPhy, U32* addrVirt)
{
	MMAP_ADDR_ASSERT(addrPhy);
	int dev_fd = 0;
	U8 * map_base = NULL;
	int ret = 0;
	ret = _MemMap(&dev_fd, &map_base, ADDR_BASE_PAGE_ALIGNED(addrPhy));
	if(ret)
	{
		return -1;
	}
	*addrVirt = map_base;
	return dev_fd;
}

int fpgaMemUnmap(int dev_fd, U32* addrVirt)
{
	if((dev_fd == 0) || (addrVirt == NULL)){
		printf("fpgaMemUnmap： invalid paras.\n");
		return -1;
	}
	_MemUnmap(dev_fd, addrVirt);

	return 0;
}