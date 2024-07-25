CC = mipsel-elf-gcc
LD = mipsel-elf-ld
OBJCOPY = mipsel-elf-objcopy
CFLAGS = -mips32 -EL -static -Wall -Werror -g -nostdlib -fno-exceptions -fno-builtin -nostartfiles -nodefaultlibs -fno-stack-protector
OBJS = startup.o main.o
export PATH := /opt/cross/bin:$(PATH)

all: hmb2260

hmb2260: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -T hmb2260.ld -o hmb2260.elf
	$(OBJCOPY) -O binary hmb2260.elf hmb2260.bin

startup.o: startup.s
	$(CC) $(CFLAGS) -c startup.s

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *~ *.o hmb2260.elf hmb2260.bin
