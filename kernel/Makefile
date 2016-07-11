CC = gcc
CFLAGS = -m32 -MD -O2 -Wall -Werror -I./include -I./include/newlib \
		 -fno-builtin -fno-omit-frame-pointer -fno-stack-protector
ASFLAGS = -m32 -MD -I./include
LDFLAGS = -T kern.ld -m elf_i386

CSRC = $(shell find ./src -name "*.c")
SSRC = $(shell find ./src -name "*.S")

OBJS = $(SSRC:.S=.o) $(CSRC:.c=.o)
START_OBJ = start/start.o
MM_MALLOC_OBJ = src/memory/mm_malloc.o 
ALL_OBJS = $(OBJS) $(START_OBJ) 

loader: $(ALL_OBJS)
	ld $(LDFLAGS) -o loader $(START_OBJ) $(OBJS) $(MM_MALLOC_OBJ) libc.a
	objdump -D loader > code.txt

-include $(ALL_OBJS:.o=.d)

clean:
	-rm -rf code.txt loader $(ALL_OBJS) $(ALL_OBJS:.o=.d)
