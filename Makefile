CROSS_COMPILE ?= aarch64-linux-gnu-

CC	= $(CROSS_COMPILE)gcc
OBJCOPY	= $(CROSS_COMPILE)objcopy

CFLAGS = -Wall -O2 -ffreestanding -nostdlib -nostartfiles
LDFLAGS = -T linker.ld

OBJS	= boot.o gpio.o main.o
all: kernel8.img

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

gpio.o: gpio.c
	$(CC) $(CFLAGS) -c gpio.c -o gpio.o

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c -o main.o

kernel8.img: $(OBJS)
	$(CC) $(LDFLAGS) -o kernel8.elf $(OBJS)
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

clean:
	rm -f *.o *.elf kernel8.img