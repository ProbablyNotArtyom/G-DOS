//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#ifndef _HEADER_HISTORY
#define _HEADER_HISTORY

typedef struct {
	uint16_t	index;
	uint16_t	lines;
	uint16_t	max;
	char		*line_buffer;
	char 		**buff;
} history_t;

void history_init(history_t *hist, uint16_t max_lines);
void history_append(history_t *hist, char *line);
void history_up(history_t *hist);
void history_down(history_t *hist);
char *history_peek(history_t *hist);
static void trim(history_t *hist);
static void update_index(history_t *hist);
char *history_gets(history_t *hist, char *buff, int len);

#endif
