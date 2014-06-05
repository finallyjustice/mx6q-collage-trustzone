.phony: all

all: secure.bin

CC  = arm-none-linux-gnueabi-gcc
LD  = arm-none-linux-gnueabi-ld
COPY= arm-none-linux-gnueabi-objcopy
DUMP= arm-none-linux-gnueabi-objdump

secure.bin: secure.elf
	$(COPY) -O binary $< secure-origin.bin
	$(DUMP) -D $< > dump_secure.log
	$(COPY) -I binary -O binary --pad-to 0x4000 --gap-fill=0x00 secure-origin.bin $@
	
secure.elf: monitor.S secure_startup.S secure_main.c
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib monitor.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib secure_startup.S
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib secure_main.c
	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib board.c
	$(LD) -T secure.lds -o secure.elf monitor.o secure_startup.o secure_main.o board.o 

#secure.elf: *.S *.c
#	$(CC) -c -mcpu=cortex-a9 -mlittle-endian -msoft-float -Wall -Os -nostdlib $^
#	$(LD) -T secure.lds -o $@ monitor.o secure_startup.o secure_main.o board.o
	
clean:
	$(RM) *.o *.i *.bin
	$(RM) *.elf *~ *.log
	
	
