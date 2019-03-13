
	#include <std.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <io.h>

#define FLASH_DO_ERASE	0x01

uint32_t get_manufacturer_id();

//---------------------------------------------------

static const char const flash_help_txt[] = {
	"Usage: prgrom [-e] file, base\r\n"
	"       [-e erase-chip]\r\n"
};

result_t main(char *argv[], int argc){
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	uint8_t fnames;						// Track the index where file names begin
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				case 'h':
					puts(flash_help_txt);
					return RET_OK;
				case 'e':
					opts |= FLASH_DO_ERASE;
					break;
				default:
					return RET_ARGS;
			}
		} else {
			fnames = i;
			i = argc;
		}
	}
	puts("In-System flash ROM programmer, NotArtyom 2019");

	f_file file;
	f_info info;
	uint8_t *prgptr = strtoul(argv[fnames+1], NULL, 16);
	f_error res;
	uint8_t *datptr;
	uint32_t bytes;
	res = f_open(&file, argv[fnames], FA_OPEN_ALWAYS | FA_READ);
	if (res != FR_OK) return RET_ARGS;
	nprintf("File found, read %d bytes from disk", bytes);
	res = f_stat(argv[fnames], &info);
	if (res != FR_OK) return RET_ARGS;
	f_lseek(&file, 0);

	nprintf("Allocating %d bytes for file buffer.", info.fsize);
	datptr = (uint8_t*)pmalloc(info.fsize);
	res = f_read(&file, datptr, info.fsize, &bytes);
	if (res != FR_OK) return RET_ARGS;
	puts("Success.");
	nprintf("ROM device id: $%x", get_manufacturer_id());
	if (opts == FLASH_DO_ERASE)
		flash_erase();

	while (bytes > 0){
		outb(0xAA, 0x555);
		outb(0x55, 0x2AA);
		outb(0xA0, 0x555);
		*prgptr++ = *datptr++;
		while (inb(0x00000000) != inb(0x00000000)) asm(" ");
		bytes--;
	}
	return RET_OK;
}

uint32_t get_manufacturer_id(){
	outb(0xAA, 0x555);
	outb(0x55, 0x2AA);
	outb(0x90, 0x555);
	uint32_t tmp = inb(0x00000000);
	outb(0xF0, 0x00000000);
	return tmp;
}

void flash_erase(){
	puts("Erasing chip. Output will go silent.");
	outb(0xAA, 0x555);
	outb(0x55, 0x2AA);
	outb(0x80, 0x555);
	outb(0xAA, 0x555);
	outb(0x55, 0x2AA);
	outb(0x10, 0x555);
	while (inb(0x00000000) & 0x80 == 0x00) asm(" ");
	return;
}
