
	#include <std.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <cpu/io.h>
	#include <char.h>

//---------------------------------------------------

result_t main(int argc, char *argv[]){
	char buffer[512];

	puts("Standard Output (stdout): Testing...");
	puts("Standard Input (stdin): Testing...");
	printf("? ");
	gets(&buffer, 512);
	putchar('\n');
	putchar('\r');
	puts(buffer);
	return 0;
}
