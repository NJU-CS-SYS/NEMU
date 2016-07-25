CC     := gcc
LD     := ld
DUMP   := objdump

CFLAGS := -m32
CFLAGS += -std=gnu11
CFLAGS += -MD
CFLAGS += -Wall -Werror
CFLAGS += -fno-stack-protector -Wno-unused-result
CFLAGS += -I ./include
CFLAGS += -ggdb3

KERNEL_SRC := src/elf/loader.c
TESTFILE_SRC := src/elf/testfile.c

GENERATED := $(KERNEL_SRC) $(TESTFILE_SRC)
CFILES := $(GENERATED) $(filter-out $(GENERATED), $(shell find src/ -name "*.c"))

OBJS_DIR := objs
OBJS     := $(CFILES:%.c=$(OBJS_DIR)/%.o)

TESTFILE = testcase/c/trivial

LOADER_DIR = kernel
LOADER = $(LOADER_DIR)/loader

nemu: $(OBJS)
	$(CC) -o nemu $(OBJS) $(CFLAGS)
	$(DUMP) -d $@ > $@.txt

MONITOR_SRC := $(shell find monitor/ -name "*.c")
MONITOR_OBJ := $(MONITOR_SRC:.c=.o)

monitor.bin: $(MONITOR_OBJ)
	$(CC) -o $@ $(CFLAGS) $^

$(KERNEL_SRC): $(shell find kernel/* -type f -name "*.[chS]")
	cd $(LOADER_DIR) && make
	objcopy -S -O binary $(LOADER) loader
	xxd -i loader > src/elf/loader.c
	rm loader

$(TESTFILE_SRC): $(shell find `dirname $(TESTFILE)`/* -type f -name "*.[chS]")
	cd `dirname $(TESTFILE)` && make
	mv $(TESTFILE) testfile
	xxd -i testfile > $@
	rm testfile

$(OBJS_DIR)/%.o: %.c
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c $< -o $@

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
