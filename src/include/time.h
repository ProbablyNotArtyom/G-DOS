//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------


#ifndef _HEADER_TIME
#define _HEADER_TIME

	#include <stdint.h>

	
typedef int clock_t;
typedef int time_t;

struct tm {
	int tm_sec;			/* seconds,  range 0 to 59			*/
	int tm_min;			/* minutes, range 0 to 59			*/
	int tm_hour;		/* hours, range 0 to 23				*/
	int tm_mday;		/* day of the month, range 1 to 31	*/
	int tm_mon;			/* month, range 0 to 11				*/
	int tm_year;		/* The number of years since 1900	*/
	int tm_wday;		/* day of the week, range 0 to 6	*/
	int tm_yday;		/* day in the year, range 0 to 365  */
	int tm_isdst;		/* daylight saving time				*/
};

// millisecond clock
// probably needs to be more architecture dependent at some point
#define CLOCKS_PER_SEC 1000

clock_t clock(void);
double difftime(time_t end, time_t start);
time_t mktime(struct tm*);
time_t time(time_t *out);
char *asctime(const struct tm*);
char *ctime(const time_t*);
struct tm *gmtime(const time_t*);
struct tm *localtime(const time_t*);
size_t strftime(char *buf, size_t size, const char *format, const struct tm*);

#endif
