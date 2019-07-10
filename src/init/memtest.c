
	#include <std.h>
	#include <stdbool.h>
	#include <io.h>
	#include <mod/init.h>


static bool testAddr(size_t addr, uint8_t byte){
	uint8_t tmp;
	outb(byte, addr);
	tmp = inb(addr);
	if (tmp != byte){
		puts("\r\n==============================");
		printf("Mismatch at address 0x");
		printLong(addr);
		puts("\r\n==============================");
		printf("Wrote: 0x");
		printByte(byte);
		printf("\r\nRead: 0x");
		printByte(tmp);
		return false;
	} else {
		printf("\b\b");
		printByte(byte);
		return true;
	}
}

void do_memtest(size_t low, size_t high, char level){
	puts("\r\nTesting RAM");
	register size_t scan = low;
	while(scan < high){
		printf("\r\nAddress: 0x");
		printLong(scan);
		printf("      Wrote: 0x00");
		uint8_t i = 0xFF;
		if (level == 'y'){
			do {
				i++;
				if (!testAddr(scan, i)) break;
			} while (i < 0xFF);
		} else {
			if (!testAddr(scan, 0xAA)) break;
			if (!testAddr(scan, 0x55)) break;
			if (!testAddr(scan, 0x00)) break;
		}
		scan++;
	}
	puts("");
}
