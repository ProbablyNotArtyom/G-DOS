//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Standard headers for the monitor

#ifndef _HEADER_MONITOR
#define _HEADER_MONITOR

//---------------------------------------------------

	#define GMON_VERSION	"1.8"
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
				if (isVar()) var = (void*)getMonVar(*parse++); \
				else var = (void*)strToHEX();

	#define queryBreak()					\
				if (peek() == PS2_ESC)		\
					return errBREAK;

static enum errList {
		errNONE,
		errSYNTAX,
		errUNDEF,
		errNOARGS,
		errEND,
		errHEX,
		errBADRANGE,
		errBREAK,
		errDOEXIT
};

extern const enum errList const (* const funcTable[])();
extern const char* const funcNames[];
extern const char* const errors[];
extern const char const hexTable[];
extern const char const helpText[];

extern bool isCurrentVar;

void evalScript();
bool setCurrents();
bool isAddr();
bool isRange();
char* skipBlank();
char* skipToken();
char* skipHex();
bool funcCmp(const char *s1, const char *s2);
ADDRSIZE strToHEX();
enum errList throw(enum errList index);
uint32_t *getMonVar(char var);
void setMonVar(char var, uint32_t val);
bool getRange(void **lower, void **upper);
void printHex(char num);
void printByte(char num);
void printWord(uint16_t num);
void printLong(uint32_t num);

//---------------------------------------------------

#endif
