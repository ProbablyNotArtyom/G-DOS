 #ifndef _HEADER_CPU
 #define _HEADER_CPU

struct pt_regs {
	uint32_t	d1;
	uint32_t	d2;
	uint32_t	d3;
	uint32_t	d4;
	uint32_t	d5;
	uint32_t	a0;
	uint32_t	a1;
	uint32_t	a2;
	uint32_t	d0;
	uint32_t	orig_d0;
	uint32_t	stkadj;
	uint16_t	sr;
	uint32_t	pc;
	unsigned 	format :  4; /* frame format specifier */
	unsigned 	vector : 12; /* vector offset */
};

 #endif
