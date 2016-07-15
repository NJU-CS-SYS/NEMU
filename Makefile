# Cross compilation toggle

CC      = gcc
LD      = ld
DUMP    = objdump

CFLAGS  = -m32 -std=gnu11 -static -MD -Wall -Werror -fno-stack-protector -Wno-unused-result -I./include -I ./monitor -O0

CFLAGS += -ggdb3

CFILES  = src/elf/loader.c src/elf/testfile.c
CFILES := $(CFILES) $(filter-out $(CFILES), $(shell find src/ -name "*.c"))
CFILES += $(filter-out monitor/main.c, $(shell find monitor/ -name "*.c"))
OBJS    = $(CFILES:.c=.o)

TESTFILE = testcase/hello/printf

LOADER_DIR = kernel
LOADER = $(LOADER_DIR)/loader

nemu: $(OBJS)
	$(CC) -o nemu $(OBJS) $(CFLAGS)
	$(DUMP) -d $@ > $@.txt

MONITOR_SRC := $(shell find monitor/ -name "*.c")
MONITOR_OBJ := $(MONITOR_SRC:.c=.o)

monitor.bin: $(MONITOR_OBJ)
	$(CC) -o $@ $(CFLAGS) $^

src/elf/loader.c: $(shell find kernel/* -type f -name "*.[chS]")
	cd $(LOADER_DIR) && make
	objcopy -S -O binary $(LOADER) loader
	xxd -i loader > src/elf/loader.c
	rm loader

src/elf/testfile.c: $(shell find `dirname $(TESTFILE)`/* -type f -name "*.[chS]")
	cd `dirname $(TESTFILE)` && make
	mv $(TESTFILE) testfile
	xxd -i testfile > $@
	rm testfile

run: nemu
	./nemu 2>&1 | tee log.txt

gdb: nemu
	gdb ./nemu

-include $(OBJS:.o=.d)

clean:
	-rm -f nemu monitor.bin 2> /dev/null
	-rm $(OBJS)
	-cd $(LOADER_DIR) && make clean
	-cd testcase && make clean
	-rm -f $(shell find -type f -name "*.d")
	-rm -f $(shell find -type f -name "*.txt")
	-rm -f src/elf/loader.c
	-rm -f src/elf/testfile.c

opt: nemu
	perf record ./nemu

rep:
	perf report -i perf.data
