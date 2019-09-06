//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// FATROM Header

#ifndef _DEV_BSVCSERIAL_HEADER
#define _DEV_BSVCSERIAL_HEADER

//-----------------Function Protos-------------------

char bsvc_dev_peek();
char bsvc_dev_read();
charResult bsvc_dev_write(char out);
charResult bsvc_dev_init();

#endif
