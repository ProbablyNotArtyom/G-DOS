
	#include <std.h>
	#include <stdbool.h>
	#include <io.h>
	#include <mod/init.h>


void do_memtest(size_t low, size_t high){
	puts("Testing RAM");
	register size_t scan = low;
	register uint8_t tmp;
	while(scan < high){
		fputs("\r\nAddress: 0x");
		printLong(scan);
		fputs("      Wrote: 0x00");
		for (uint8_t i = 0; i < 0xFF; i++){
			outb(i, scan);
			tmp = inb(scan);
			if (tmp != i){
				puts("\r\n==============================");
				fputs("Mismatch at address 0x");
				printLong(scan);
				puts("\r\n==============================");
				fputs("Wrote: 0x");
				printByte(i);
				fputs("\r\nRead: 0x");
				printByte(tmp);
				return;
			} else {
				fputs("\b\b");
				printByte(i);
			}
		}
		scan++;
	}
}
