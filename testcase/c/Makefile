CC      = gcc
LD		= ld
CFLAGS  = -m32 -Wall -Werror -static -I.. -fno-stack-protector
#LDFLAGS = -melf_i386 -e main -Ttext-segment=0x800000
LDFLAGS = -melf_i386 -e main

SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=)

all: $(OBJS)

$(OBJS): % : %.o
	$(LD) $(LDFLAGS) -o $@ $<
	objdump -d $@ > $@.txt

clean:
	-rm -f $(OBJS) $(SRCS:.c=.o) $(SRCS:.c=.txt) 2> /dev/null
