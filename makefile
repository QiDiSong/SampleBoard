#CC		= gcc
CC		= armeb-linux-gnueabi-gcc
STD		= 
TARGET  = tcp_demo
CFLAGS  = -g -Wall -w
RM      = rm

## source file path
SRC_PATH   := .

_XUE_COMPILE_DATE = $(shell date +"%Y-%m-%d")
_XUE_COMPILE_TIME = $(shell date +"%H:%M:%S")
$(warning "XUE_COMPILE_DATE=$(_XUE_COMPILE_DATE)")
$(warning "XUE_COMPILE_TIME=$(_XUE_COMPILE_TIME)")

CFLAGS += -DXUE_COMPILE_DATE=$(_XUE_COMPILE_DATE)
CFLAGS += -DXUE_COMPILE_TIME=$(_XUE_COMPILE_TIME)
# CFLAGS += -lpthread 

#_TARGET_GIT_BRANCH = $(shell `sh -c 'git branch --no-color 2> /dev/null' | sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/' -e 's/\//\_/g'`)
#$(warning "_TARGET_GIT_BRANCH=$(_TARGET_GIT_BRANCH)")

#CFLAGS += -DTARGET_GIT_BRANCH=$(_TARGET_GIT_BRANCH)

## get all source files
SRC += $(wildcard $(SRC_PATH)/*.c)
INC += $(wildcard $(SRC_PATH)/*.h)

##  $@:目标文件  $^ :所有的依赖文件  $< :第一个依赖文件  $? :比目标还要新的依赖文件列表
%.o : %.c
	$(CC) $(STD) $(CFLAGS) -c $< -o $@

## all .o based on all .c
OBJ = $(patsubst %c, %o, $(SRC))

all:$(OBJ)
	$(CC) $(OBJ) -lpthread -o $(TARGET)

 
clean:
	$(RM) $(TARGET) $(OBJ)
