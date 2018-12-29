//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

//---------------------Includes----------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <fs.h>

//-----------------Variable Protos-------------------



//-----------------------Main------------------------

int main(void){
	if (init() == -1){
		puts("Init sequence failed!\n\rHaulting execution...");
		while (1);
	}

	fsInit();
	initcall_t *fn = &__start_initsec;
	while (fn < &__stop_initsec){
		puts("pass");
		(*fn)();
		fn++;
	}

	puts("NotArtyom 02/11/18");
	puts("G'DOS Shell");
	sirpinski(5);
	monBegin();

	puts("End.");
	while(1);
}

#define SIZE (1 << 4)
void sirpinski(int num){
	int x, y, i;
	for(num; num > 0; num--){
		for (y = SIZE - 1; y >= 0; y--, puts("")) {
			for (i = 0; i < y; i++) putc(' ');
			for (x = 0; x + y < SIZE; x++)
				printf((x & y) ? "  " : "* ");
			}
			for (y = 1; y <= SIZE; y++, puts("")) {
				for (i = 0; i < y; i++) putc(' ');
				for (x = 0; x + y < SIZE; x++)
				printf((x & y) ? "  " : "* ");
			}
	}
}
