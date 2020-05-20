APP = sampleapp
ROOT = `pwd`
myslp = /home/hasitha/MegaLive/MegaLiveProj/build/tmp/sysroots-components/cortexa9t2hf-neon/openslp/usr/include #$(ROOT)/../openslp_zynq

LINUX_SVC = LSVC
COMMON = common

INCLUDE_DIRS = $(ROOT) \
               $(ROOT)/$(LINUX_SVC) \
               $(ROOT)/$(COMMON) \
               $(myslp)/include

C_INCLUDE = $(foreach INCLUDE_PATH, $(INCLUDE_DIRS), -I$(INCLUDE_PATH))

src = $(wildcard LSVC/*.c) \
      $(wildcard common/*.c) \
      $(wildcard *.c)
 
APP_OBJS = $(src:.c=.o)

CFLAGS = -D__RTXC__ \
        -D_GNU_SOURCE \
         $(C_INCLUDE)

LDFLAGS = -pthread -lrt -lm -lslp

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS) -Wl,-rpath-link, -Wl,-O1 -Wl,--hash-style=gnu -Wl,--allow-multiple-definition

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@


.PHONY: clean
clean:
	rm -f $(wildcard *.o) sampleapp
