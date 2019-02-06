 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // PS/2 Keyboard Driver

 #ifdef CONFIG_DEV_DUMMY
 #define CONFIG_DEV_DUMMY

	#include <std.h>
	#include "dummy.h"

//---------------------------------------------------



//--------------------Functions----------------------

charResult dummy_dev_write(char out){
	return CH_NOTRDY;
}

char dummy_dev_read(){
	while(1);
}

charResult dummy_dev_init(){
	return CH_OK;
}

 #endif
