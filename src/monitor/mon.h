//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Standard headers for the monitor

#ifndef _HEADER_MONITOR
#define _HEADER_MONITOR

//---------------------------------------------------

	#define	BUFFLEN		0xFF
	#define NUMFUNCS	8

	#define	ADDRSIZE	uint32_t

//---------------------------------------------------

	#define ifEOI(err)						\
				if (*skipBlank() == '\0')	\
					return err;

	#define getArg(var)						\
				if (*skipBlank() == '\0')	\
					return errNOARGS;		\
				var = (void*)strToHEX();

static enum errList {
		errNONE,
		errSYNTAX,
		errUNDEF,
		errNOARGS,
		errEND,
		errHEX,
		errBADRANGE
	};

extern const enum errList const (*funcTable[])();
extern const char* const funcNames[];
extern const char* const errors[];
extern const char const hexTable[];

char* skipBlank();
char* skipToken();
char* skipHex();
bool funcCmp(const char *s1, const char *s2);
ADDRSIZE strToHEX();
void throw(enum errList index);

//---------------------------------------------------

#endif
