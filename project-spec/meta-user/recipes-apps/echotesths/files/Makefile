 
APP = echotesths  
APP_OBJS = echotesths.o rpmsg_utility.o sharememechotest.o


# Add any other object files to this list below
INCLUDE_DIRS = /home/hasitha/MegaLive/MegaLiveProj/project-spec/meta-user/recipes-apps/echotesths/files

C_INCLUDE = $(foreach INCLUDE_PATH, $(INCLUDE_DIRS), -I$(INCLUDE_PATH))

# Add any other object files to this list below

#CC:=arm-linux-gnueabihf-gcc #need to remove this for petalinux-build
all: $(APP)

$(APP): $(APP_OBJS)  
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS)  

clean:
	rm -rf $(APP) *.o

%.o: %.c 
	$(CC) -c $(CFLAGS) -o $@ $< $(DEPS)

