//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#define	BUFFLEN		0xFF
	#define NUMFUNCS	7

	#define	ADDRSIZE	uint32_t

//---------------------Includes----------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	
	#include "mon.h"

//-----------------------Main------------------------

bool doExit;
char* parse;

void monBegin(){
	doExit = false;
	static char inBuffer[BUFFLEN];				// Our input buffer
	puts("G'Mon Version 0.1");
	while (doExit == false){
		fputs("> ");							// Print the prompt
		parse = inBuffer;						// Set the parse pointer to the beginning of the buffer
		gets(inBuffer, BUFFLEN);				// Get user input
		skipBlank();							// Skip and leading spaces
		uint8_t i;
		for (i = 0; funcCmp(parse, funcNames[i]) == false && i < NUMFUNCS; i++);	// Identify what function it is
		if (i == NUMFUNCS) throw(errUNDEF);		// If none matches, complain
		else {
			skipToken();						// Skip over the function name itself
			throw((*funcTable[i])());			// Call the function from out lookup table using i as out index
		}										// Call it from a throw so we can print out any errors after returning
	}
	return;
}

/* Exits the monitor */
static enum errList exit(){
	doExit = true;										// Sets the exit flag true 
	return errNONE;										// and returns without error
}

/* Writes bytes to memory */
static enum errList deposit(){
	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no given arguments
	char *ptr = strToHEX();								// Take the argument in as the starting memory pointer
	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments

	while (*parse != '\0'){								// Keep reading in arguments until we hit the end of input
		*ptr++ = strToHEX();							// Take those arguments and store them to memory in succesion
		skipBlank();
	}
	return errNONE;										// Return error free
}

/* Handles viewing of memory */
static enum errList read(){
	char *ptr, *end;									// Create start and end pointers

	if (*skipBlank() == '\0') return errNOARGS;			// Error if there arent any inputs
	while (*parse != '.' && *parse != '\0'){			// Keep reading in values to print until a range identifier is found, if at all
		ptr = strToHEX();									// Get the location to read from
		skipBlank();										// Go ahead and align with the next argument
		puts("");											// Print a newline
		printLong(ptr);										// Print the memory location itself
		fputs(" | ");										// Fancy seperators
		printByte(*ptr++);									// Print the data at that memory location
		putc(' ');											// Cool spaces
	} 

	if (*parse == '.'){									// If we hit a range identifier...
		parse++;										// Then skip over it
		skipBlank();									// And move to the next argument
		uint8_t column = 1;								// Create something to track how many columns have been printed so far
		if (*skipBlank() == '\0') return errNOARGS;		// Error if there arent more args
		end = strToHEX();								// Read in the end of the range
		if (end == NULL) return errHEX;					// Error out if end is 0, thats wack
		while (ptr != end){								// Continue until we've reached the end of the range
			if (column == 16){								// If we've printed 16 columns,
				puts("");									// Then set up a new line
				column = 0;									// And print out the location header
				printLong(ptr);
				fputs(" | ");
			}
			printByte(*ptr);								// Print data byte at this address
			column++;										// Increase our column number
			putc(' ');										// Space between bytes
			ptr++;											// Increase the data pointer
		}
	}
	return errNONE;										// Return error free
}

/* Copies a range of data */
static enum errList copy(){
	char *ptr, *end, *dest;								// Create pointers for start, end, and destination of block

	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments
	ptr = strToHEX();									// Store the next argument as the start pointer
	if (*skipBlank() != '.') return errBADRANGE;		// Error if there is no range identifier
	parse++;											// Step over the range identifier '.'
	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments
	end = strToHEX();									// Store the next argument as the end pointer
	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments
	dest = strToHEX();									// Store the next argument as the destination pointer

	if (dest <= ptr) {									// If the destination is below the source in memory,
		while (ptr <= end) *dest++ = *ptr++;			// then copy it starting at the beginning
	} else {
		dest += end - ptr;								// If the destination is above the start of the source,
		while (end >= ptr) *dest-- = *end--;			// then copy starting at the end of the source
	}													// This is done to avoid overwriting the source before we can copy it

	return errNONE;										// Return error free
}

/* Moves a block of data, filling the old space with 00 */
static enum errList move(){
	char *ptr, *end, *dest;								// Create pointers for start, end, and destination of block

	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments
	ptr = strToHEX();									// Store the next argument as the start pointer
	if (*skipBlank() != '.') return errBADRANGE;		// Error if there is no range identifier
	parse++;											// Step over the range identifier '.'
	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments
	end = strToHEX();									// Store the next argument as the end pointer
	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments
	dest = strToHEX();									// Store the next argument as the destination pointer

	if (dest <= ptr) {									// If the destination is below the source in memory,
		while (ptr <= end) *dest++ = *ptr++;			// then copy it starting at the beginning
	} else {
		dest += end - ptr;								// If the destination is above the start of the source,
		while (end >= ptr) *dest-- = *end--;			// then copy starting at the end of the source
	}													// This is done to avoid overwriting the source before we can copy it

	return errNONE;										// Return error free
}

/* Fills a range with a pattern byte */
static enum errList fill(){
	char *ptr, *end;									// Create pointers for the start and end of the section
	uint8_t val;										// The fill pattern itself

	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments
	ptr = strToHEX();									// Store the next argument as the start pointer
	if (*skipBlank() != '.') return errNOARGS;			// Error if the next argument isn't a range identifier
	parse++;											// Step over the identifier
	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments
	end = strToHEX();									// Store the next argument as the end pointer
	if (*skipBlank() == '\0') return errNOARGS;			// Error if there are no more arguments
	val = strToHEX();									// Store whats left as the value to use as the fill

	while (ptr <= end) *ptr++ = val;					// Set every byte from *ptr to *end to the pattern in val

	return errNONE;										// Return error free
}

/* Starts executing code from a place in memory */
static enum errList execute(){
	if (*skipBlank() == '\0') return errNOARGS;			// Error if no address is given
	void (*ptr)(void) = strToHEX();						// Create a function pointer that points to wherever the next argument tells it to
	(*ptr)();											// Call that function 
	return errNONE;										// Return error free, assuming that whatever we call actually returns (good chance it wont)
}

//----------------------Tables-----------------------

const enum errList const (*funcTable[])() = {
	exit,
	deposit,
	read,
	copy,
	move,
	fill,
	execute,
	NULL
};

const char* const funcNames[] = {
	"exit ",
	"put ",
	"read ",
	"copy ",
	"move ",
	"fill ",
	"go ",
	"\0"
};

const char* const errors[] = {
	"",
	"\n\r?Syntax error",
	"\n\r?Undefined function",
	"\n\r?Unexpected arguments",
	"\n\r!Unexpected end of input",
	"\n\r!Invalid hex",
	"\n\r?Invalid range"
};

const char const hexTable[] = "0123456789ABCDEF";