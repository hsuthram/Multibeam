#CC:=arm-linux-gnueabihf-gcc
#CC:=arm-xilinx-linux-gnueabi-gcc

ROOT = `pwd`

myslp = $(ROOT)/../openslp_zynq

LINUX_SVC = LSVC
COMMON = common
#FABRIC = fabric

INCLUDE_DIRS = $(ROOT) \
               $(ROOT)/$(LINUX_SVC) \
               $(ROOT)/$(COMMON) \
           #    $(ROOT)/$(FABRIC) \
	       $(myslp)/include

C_INCLUDE = $(foreach INCLUDE_PATH, $(INCLUDE_DIRS), -I$(INCLUDE_PATH))

src = $(wildcard LSVC/*.c) \
      $(wildcard common/*.c) \
 #     $(wildcard fabric/*.c) \
      $(wildcard *.c)

obj = $(src:.c=.o)

CFLAGS = -D__RTXC__ \
	 -D_GNU_SOURCE \
         $(C_INCLUDE)

LDFLAGS = -pthread -lrt -lm -L$(myslp)/lib -lslp

mbsonar: $(obj) 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) mbsonar
