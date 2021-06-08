//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <stdlib.h>

	#include <lib/sysexit.h>
	#include <history.h>
	#include <tui.h>

//---------------------------------------------------

void history_init(history_t *hist, uint16_t max_lines) {
	hist->max = max_lines;
	hist->lines = 1;
	hist->buff = (char **)malloc(sizeof(char *) * max_lines);
	update_index(hist);
	hist->buff[hist->index] = "";
	update_index(hist);
}

void history_append(history_t *hist, char *line) {
	update_index(hist);
	hist->buff[hist->index] = (char *)malloc(strlen(line));
	strcpy(hist->buff[hist->index], line);
	hist->lines += 1;
	update_index(hist);
	hist->buff[hist->index] = "";
	update_index(hist);
}

void history_up(history_t *hist) {
	hist->index = (hist->index > 0)? hist->index-1 : hist->index;
}

void history_down(history_t *hist) {
	hist->index = (hist->index < hist->lines-1)? hist->index+1 : hist->index;
}

char *history_peek(history_t *hist) {
	return hist->buff[hist->index];
}

static void trim(history_t *hist) {
	char *tmp = hist->buff[hist->lines-1];
	for (int i = hist->lines-1; i > 0; i--) {
		hist->buff[i] = hist->buff[i-1];
	}
	hist->buff[0] = tmp;
}

static void update_index(history_t *hist) {
	hist->index = hist->lines - 1;
	hist->line_buffer = hist->buff[hist->index];
}

//---------------------------------------------------

char *history_gets(history_t *hist, char *buff, int len) {
	char *s = buff, *s_far = buff;
	char tmp, input_dirty = false;

	void make_clean() {
		if (!input_dirty) return;		// Do nothing unless the input is actually dirty
		strncpy(buff, hist->buff[hist->index], len);
		s = buff + strlen(hist->buff[hist->index]);
		s_far = s;
		input_dirty = false;
	}

	memset(buff, NULL, len);
	tui_cursor_save();
	while (true) {
		tmp = getchar();

		if (tmp == '\r') {			// CR always marks the end of the input, so handle it first
			make_clean();
			*s_far = NULL;
			putchar(tmp);
			if (buff == s_far) break;

			history_append(hist, buff);
			break;
		}

		if (tmp == '\033') {
			tmp = getchar();
			tmp = getchar();
			switch(tmp) {
				case 'A':	// code for arrow up
					history_up(hist);
					break;
				case 'B':	// code for arrow down
					history_down(hist);
					break;
				case 'C':	// code for arrow
					make_clean();
					if (s_far > s) {
						s++;
						tui_cursor_right(1);
					}
					continue;
				case 'D':	// code for arrow left
					make_clean();
					if (buff < s) {
						s--;
						tui_cursor_left(1);
					}
					continue;
			}

			if (hist->index < hist->lines - 1) {
				input_dirty = true;
				tui_cursor_restore();
				tui_clear_line_right();
				printf(hist->buff[hist->index]);
			} else {
				input_dirty = false;
				tui_cursor_restore();
				tui_clear_line_right();
				nputs(buff, s_far - buff);
			}
		} else {
			make_clean();
			if (tmp == 0x7F || tmp == 0x08) {
				if (buff < s) {
					printf("\b \b");
					*s = NULL;
					if (s == s_far) s_far--;
					s--;
				}
			} else {
				*s = tmp;
				if (s < (buff + len - 1)) {
					putchar(*s);
					if (s == s_far) s_far++;
					s++;
				}
			}
		}
	}

	return *buff;
}

//---------------------------------------------------
