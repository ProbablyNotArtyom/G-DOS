//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// G'DOS Standard things for string manipulation

	#include <std.h>
	#include <stddef.h>
	#include <stdarg.h>

//---------------------------------------------------

int isSpace(int val){
	return ((char)val == ' ') ||
		((char)val == '\n') ||
		((char)val == '\r') ||
		((char)val == '\t');
}

int isLower(int val){
	return ((char)val <= 'z' && (char)val >= 'a');
}

int isUpper(int val){
	return ((char)val <= 'Z' && (char)val >= 'A');
}

int isAlpha(int val){
	return (isUpper(val) || isLower(val));
}

int isDigit(int val){
	return ((char)val <= '9' && (char)val >= '0');
}

int toUpper(int val){
	if ((val >= 'a') && (val <= 'z')){
		return (val - 0x20);
	}
	return val;
}

int toLower(int val){
	if ((val >= 'A') && (val <= 'Z')){
		return (val + 0x20);
	}
	return val;
}

int isAscii(int val)
{
	return !(( char)val & 0x80);
}

int isAlnum(int val)
{
	return isDigit(val) || isAlpha(val);
}
