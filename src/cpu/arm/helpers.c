
	#include <stdint.h>

void copy_vectors(void) {
	extern uint32_t vectors_start;
	extern uint32_t vectors_end;
	uint32_t *vectors_src = &vectors_start;
	uint32_t *vectors_dst = (uint32_t *)0;

	while(vectors_src < &vectors_end)
		*vectors_dst++ = *vectors_src++;
}
