
	#include <std.h>
	#include <stdbool.h>
	#include <io.h>
	#include <mod/init.h>

//---------------------------------------------------

extern const char __RAM_START, __RAM_END;
extern const char _end;

//---------------------------------------------------

static bool testAddr(size_t addr, uint8_t byte){
	uint8_t tmp;
	outb(byte, addr);
	tmp = inb(addr);
	if (tmp != byte){
		puts("\n==============================");
		printf("Mismatch at address 0x");
		printLong(addr);
		puts("\n==============================");
		printf("Wrote: 0x");
		printByte(byte);
		printf("\nRead: 0x");
		printByte(tmp);
		return false;
	} else {
		printf("\b\b");
		printByte(byte);
		return true;
	}
}

static void do_memtest(size_t low, size_t high, char level){
	puts("\nTesting RAM");
	register size_t scan = low;
	while(scan < high){
		printf("\nAddress: 0x");
		printLong(scan);
		printf("      Wrote: 0x00");
		uint8_t i = 0xFF;
		if (level == 'n' || level == 'N'){
			if (!testAddr(scan, 0xAA)) break;
			if (!testAddr(scan, 0x55)) break;
			if (!testAddr(scan, 0x00)) break;
		} else do {
			i++;
			if (!testAddr(scan, i)) break;
		} while (i < 0xFF);
		scan++;
	}
	puts("");
}

//---------------------------------------------------

void memtest_start() {
	size_t test_start = &_end;
	size_t test_end = &__RAM_END;
	char buff0[9];

	memset(buff0, NULL, 9);
	printf("[.] Start (default 0x%X) : 0x", test_start);
	gets(buff0, 9);
	printf("\n");
	if (buff0[0]) test_start = strtoul(&buff0, NULL, 16);

	memset(buff0, NULL, 9);
	printf("[.] End (default 0x%X) : 0x", test_end);
	gets(buff0, 9);
	printf("\n");
	if (buff0[0]) test_end = strtoul(&buff0, NULL, 16);

	printf("[?] Intense (Y/n) : ");
	do_memtest(test_start, test_end, getchar());
}
