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

	#define GMON_VERSION	"1.2"
	#define	BUFFLEN			0xFF

	#define	ADDRSIZE	uint32_t

//---------------------------------------------------

	#define isEOI()							\
				(*skipBlank() == '\0')

	#define ifEOI(err)						\
				if (*skipBlank() == '\0')	\
					return err;

	#define getArg(var)						\
				if (*skipBlank() == '\0')	\
					return errNOARGS;		\
				var = (void*)strToHEX();

	#define getRange(lower, upper)			\
				(*skipBlank() == '\0');		\
				if (isAddr()) lower = strToHEX(); \
				else if (*parse == '.' || *parse == ',') lower = current_addr; \
				else return errSYNTAX;		\
				if (*parse == '.'){			\
					parse++;				\
					skipBlank();			\
					ifEOI(errNOARGS);		\
					if (!isAddr()) return errSYNTAX; \
					upper = strToHEX();		\
				}							\
				else if (*parse == ','){			\
					parse++;				\
					skipBlank();			\
					ifEOI(errNOARGS);		\
					if (!isAddr()) return errSYNTAX; \
					upper = (strToHEX() + (uint32_t)lower);		\
				} else {										\
					upper = NULL;								\
				}

static enum errList {
		errNONE,
		errSYNTAX,
		errUNDEF,
		errNOARGS,
		errEND,
		errHEX,
		errBADRANGE,
		errDOEXIT
};

extern const enum errList const (* const funcTable[])();
extern const char* const funcNames[];
extern const char* const errors[];
extern const char const hexTable[];
extern const char const helpText[];

void evalScript();
void setCurrents();
bool isAddr();
bool isRange();
char* skipBlank();
char* skipToken();
char* skipHex();
bool funcCmp(const char *s1, const char *s2);
ADDRSIZE strToHEX();
enum errList throw(enum errList index);

//---------------------------------------------------

#endif
