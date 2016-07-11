# Cross compilation toggle

# setting compiler and compile options
# ARCH 前缀在特定的 target 下会设置成交叉编译工具链的前缀
CC      = $(ARCH)gcc
LD      = $(ARCH)ld
DUMP    = $(ARCH)objdump

CFLAGS  = -std=gnu11 -static -MD -Wall -Werror -fno-stack-protector -Wno-unused-result -I./include -I ./monitor -O0

CFLAGS += -ggdb3

CFILES  = src/elf/loader.c src/elf/testfile.c
CFILES := $(CFILES) $(filter-out $(CFILES), $(shell find src/ -name "*.c"))
CFILES += $(filter-out monitor/main.c, $(shell find monitor/ -name "*.c"))
OBJS    = $(CFILES:.c=.o)

TESTFILE = testcase/hello/read

LOADER_DIR = kernel
LOADER = $(LOADER_DIR)/loader

LIBC = $(shell $(CC) -print-file-name=libc.a)
LIBGCC = $(shell $(CC) -print-file-name=libgcc.a)
LIBGCC_EH = $(shell $(CC) -print-file-name=libgcc_eh.a)

# 在特定目标下使用特定类型工具链，减少对 Makefile 的修改
#nemu_on_npc.o nemu_on_npc: ARCH = mips-linux-gnu-
#nemu_on_npc.o nemu_on_npc: CFLAGS += -D DEPLOY

# 只产生可重定位文件，据此先统计不引入标准库时指令使用情况，
# 然后观察需要提供哪些标准库函数(LIGGCC 用于提供 GCC 内置函数实现）。
nemu_on_npc.o: $(filter-out monitor/monitor.o, $(OBJS))
	$(LD) -r $^ -o $@
	$(DUMP) -d $@ > $@.txt

# 生成用户态 MIPS 大端目标代码
#$(LD) -m elf32btsmip -e main $(filter-out monitor/monitor.o, $(OBJS)) $(LIBC) $(LIBGCC) $(LIBGCC_EH) $(LIBC) -o mips-nemu
nemu_on_npc: $(filter-out monitor/monitor.o, $(OBJS))
	$(LD) -m elf32btsmip -e main $^ $(LIBGCC) -o $@
	$(DUMP) -d $@ > $@.txt

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
	$(DUMP) -d loader > loader.txt
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
	-rm -f nemu monitor.bin nemu_on_npc nemu_on_npc.o 2> /dev/null
	-rm $(OBJS)
	-cd $(LOADER_DIR) && make clean
	-cd testcase && make clean
	-rm -f $(shell find -type f -name "*.d")
	-rm -f $(shell find -type f -name "*.txt")
	-rm -f $(shell find -type f -name "*.map")
	-rm -f src/elf/loader.c
	-rm -f src/elf/testfile.c

opt: nemu
	perf record ./nemu

rep:
	perf report -i perf.data
