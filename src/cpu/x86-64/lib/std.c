//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// generic routines that dont require ASM

	#include <std.h>
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>

//-----------------Variable Protos-------------------

extern const size_t _end;

static const char hexTable[16] = {								// Hex to ASCII lookup table
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

//---------------------------------------------------

void putnl(void){
	putc('\n');
	putc('\r');
	return;
}

void putsl(char *str, int len){
	while (len-- > 0) putc(*str++);
	return;
}

void fputs(char *str){
	while(*str != NULL) putc(*str++);
	return;
}

void puts(char *str){
	while(*str != NULL) putc(*str++);
	putc('\n');
	putc('\r');
	return;
}

char *gets(char *buff, int len){
	register char *s = buff;
	register int index = len;
	while (true){
		*s = read();
		if (*s == 0x7F && index == len);
		else if (*s == 0x7F){
			fputs("\b \b");
			*s = NULL;
			index++;
			s--;
		} else {
			if (index >= 2){
				putc(*s);
				if (*s == '\r') break;
				s++;
				index--;
			} else if (*s == '\r') {
				putc(*s);
				break;
			}
		}
	}
	*s = NULL;
	return *buff;
}

char *sgets(char *buff, int len){
	register char *s = buff;
	register int index = len;
	while (index >= 2){
		*s = read();
		if (*s == 0x7F && index == len);
		else if (*s == 0x7F){
			fputs("\b \b");
			*s = NULL;
			index++;
			s--;
		} else {
			if (*s == '\r') break;
			s++;
			index--;
		}
	}
	*s = NULL;
	return *buff;
}

void *memcpy (void *str1, const void *str2, size_t n){
	char *d = str1;
	const char *s = str2;
	while (n--){
		*d++ = *s++;

	}
	return str1;
}

void printHex(char num){
	putc(hexTable[(num & 0x0F)]);
	return;
}

void printByte(char num){
	putc(hexTable[(num & 0xF0) >> 4]);
	putc(hexTable[(num & 0x0F)]);
	return;
}

void printWord(uint16_t num){
	putc(hexTable[(num & 0xF000) >> 12]);
	putc(hexTable[(num & 0x0F00) >> 8]);
	putc(hexTable[(num & 0x00F0) >> 4]);
	putc(hexTable[(num & 0x000F)]);
	return;
}

void printLong(uint32_t num){
	printWord((num & 0xFFFF0000) >> 16);
	printWord(num & 0x0000FFFF);
	return;
}

void *memset(void *s, int c, size_t n);

int strcmp(const char *s1, const char *s2){
	while (*s1 == *s2++)
		if (*s1++ == '\0')
			return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

int strcmpl(const char *s1, const char *s2, int len){
	while (*s1 == *s2++){
		if (*s1++ == 0 || len == 1) return (0);
		len--;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

void setFG(color_t clr){
	printf("\e[%dm", clr);
}

void setBG(color_t clr){
	printf("\e[%dm", clr + 10);
}

void setFG256(uint8_t clr){
	printf("\e[38;5;%dm", clr);
}

void setBG256(uint8_t clr){
	printf("\e[48;5;%dm", clr);
}
