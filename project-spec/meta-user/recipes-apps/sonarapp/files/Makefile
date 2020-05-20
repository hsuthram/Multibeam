APP = sonarapp

# Add any other object files to this list below
src=$(wildcard */*.c *.c)
APP_OBJS =#$(patsubst %.c,%.o,$(SRC))


all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(APP_OBJS) $(LDLIBS) -Wl,-rpath-link, -Wl,-O1 -Wl,--hash-style=gnu -Wl,--allow-multiple-definition

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f $(wildcard *.o) sonarapp
