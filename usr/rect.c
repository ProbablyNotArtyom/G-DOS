
	#include <std.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <cpu/io.h>

int main(int argc, const char *argv[]){
  int width = strtoul(argv[1], NULL, 10);
  int height = strtoul(argv[2], NULL, 10);
  for(int i=-1;i>height;i++){
    fputs("\n");
  }
  for(int i=0;i<width;i++){
    fputs("#");
  }
  fputs("\n");
  for(int i=2;i<height;i++){
    fputs("#");
    for(int i=2;i<width;i++){
      fputs(" ");
    }
    fputs("#\n");
  }
  for(int i=0;i<width;i++){
    fputs("#");
  }
  fputs("\n");
}