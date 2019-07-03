
	#include <std.h>
	#include <cpu/calling.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <cpu/io.h>
	#include <char.h>

//---------------------------------------------------

static const char const help_txt[] = {
	"Brot renders a mandelbrot figure out to the terminal\r\n"
	"Usage: brot \r\n"
};

result_t main(char *argv[], int argc){
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	uint8_t fnames;
	uint32_t num;
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				case 'h':
					puts(help_txt);
					return RET_OK;
				default:
					return RET_ARGS;
			}
		} else {
			fnames = i;
			i = argc;
		}
	}
	num = strtoul(argv[fnames], NULL, 10);

	while (num > 0){
		int n = 0;
		float r,i,R,I,b;
		for(i=-1;i<1;i+=.06,puts(""))for(r=-2;I=i,(R=r)<1;
		r+=.040,putc(n+31))for(n=0;b=I*I,26>n++&&R*R+b<4;I=2*R*I+i,R=R*R-b+r);
		num--;
	}
}
