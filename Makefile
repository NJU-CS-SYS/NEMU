# Cross compilation toggle
#ARCH = mips-linux-gnu-

# setting compiler and compile options
CC      = $(ARCH)gcc
LD      = $(ARCH)ld
DUMP    = $(ARCH)objdump

CFLAGS  = -MD -Wall -Werror -fno-strict-aliasing -Wno-unused-result -I./include -I ./monitor -O3 -static

# target to compile
CFILES  = $(shell find src/ -name "*.c")
CFILES += $(filter-out monitor/main.c, $(shell find monitor/ -name "*.c"))
OBJS    = $(CFILES:.c=.o)

# test files
TESTFILE = testcase/c/quick-sort
C_TEST_FILE_LIST = $(shell find testcase/c/ -name "*.c")
S_TEST_FILE_LIST = $(shell find testcase/asm/ -name "*.S")
TEST_FILE_LIST = $(C_TEST_FILE_LIST:.c=) $(S_TEST_FILE_LIST:.S=)

LOADER_DIR = kernel
LOADER = $(LOADER_DIR)/loader

nemu: $(LOADER)
nemu: $(OBJS) $(LOADER)
	$(CC) -o nemu $(OBJS) $(CFLAGS)

MONITOR_SRC := $(shell find monitor/ -name "*.c")
MONITOR_OBJ := $(MONITOR_SRC:.c=.o)

monitor: $(MONITOR_OBJ)
	$(CC) -o monitor.bin $(CFLAGS) $^

$(TEST_FILE_LIST):
	cd `dirname $@` && make

loader $(LOADER):
	cd $(LOADER_DIR) && make
	objcopy -S -O binary $(LOADER_DIR)/loader loader
	xxd -i loader > src/elf/loader.c
	rm loader

run: nemu $(TESTFILE)
	./nemu $(TESTFILE) 2>&1 | tee log.txt

gdb: nemu $(TESTFILE)
	gdb --args ./nemu $(TESTFILE)

test: nemu $(TEST_FILE_LIST)
	bash all.sh $(TEST_FILE_LIST)

-include $(OBJS:.o=.d)

clean:
	-rm -f nemu monitor.bin 2> /dev/null
	-rm $(OBJS)
	-cd $(LOADER_DIR) && make clean
	-cd testcase && make clean
	-rm -f $(shell find -type f -name "*.d")
	-rm -f $(shell find -type f -name "*.txt")
	-rm -f $(shell find -type f -name "*.map")

opt:
	perf record ./nemu $(TESTFILE)

rep:
	perf report -i perf.data
