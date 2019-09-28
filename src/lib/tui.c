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

	#include <tui.h>

//---------------------------------------------------

void tui_cls() {
	printf("\e[H\e[2J\e[3J\ec" COLOR_RESET_BG COLOR_RESET_FG);
}

void tui_cursor_up(const int len) {
	printf("\e[%dA", len);
}

void tui_cursor_down(const int len) {
	printf("\e[%dB", len);
}

void tui_cursor_left(const int len) {
	printf("\e[%dD", len);
}

void tui_cursor_right(const int len) {
	printf("\e[%dC", len);
}

void tui_cursor_setpos(const int x, const int y) {
	printf("\e[%d;%dH", y, x);
}

void tui_mode_set(const int mode) {
	printf("\e#%d", mode);
}

void tui_color_fg(unsigned char color, char *str) {
	printf("\e[%dm%s" COLOR_RESET_FG, color, str);
}

void tui_color_bg(unsigned char color, char *str) {
	printf("\e[%dm%s" COLOR_RESET_BG, color+10, str);
}

void tui_set_color_fg_bg(unsigned char fg, unsigned char bg) {
	printf("\e[%dm\e[%dm", fg, bg+10);
}

void tui_reset_color_fg_bg() {
	//printf(COLOR_RESET_BG COLOR_RESET_FG);
	printf("\e[%dm\e[%dm", C_LIGHTGREY, C_BLACK_BG);
}

void tui_reset_color_fg() {
	//printf(COLOR_RESET_FG);
	printf("\e[%dm", C_LIGHTGREY);
}

void tui_reset_color_bg() {
	//printf(COLOR_RESET_BG);
	printf("\e[%dm", C_BLACK_BG);
}

/* returns the state */
char tui_mouse_get_event(point_t *point) {
	char tmp;
	while (tmp != 'M') {
		while (tmp != '[') {
			while (tmp != '\e') tmp = getchar();
			tmp = getchar();
		}
		tmp = getchar();
	}
	tmp = getchar();
	point->x = getchar()-32;
	point->y = getchar()-32;
	return tmp;
}

char tui_mouse_get_click_l(point_t *point) {
	struct gfx_box hlight = { 1, 1 };
	char tmp;

	while (tmp != 32) tmp = tui_mouse_get_event(point);
	tui_set_sgr(SGR_INVERSE);
	hlight.x = point->x, hlight.y = point->y;
	draw_gfx_box(&hlight, COLOR_DEFAULT_FG, COLOR_DEFAULT_BG);

	while (tmp != 35) {
		tmp = tui_mouse_get_event(point);
	}
	tui_unset_sgr(SGR_INVERSE);
	draw_gfx_box(&hlight, COLOR_DEFAULT_FG, COLOR_DEFAULT_BG);
	return tmp;
}

//-------------------libTMenu------------------------

menu_list_t *new_menu_list(unsigned int numEntries, plot_t height, plot_t width, plot_t x, plot_t y) {
	menu_list_t *list = (menu_list_t *)malloc(sizeof(menu_list_t));
	if (list == NULL) return NULL;
	if (numEntries == NULL || numEntries < 1) {
		list->numEntries = 0;
		return list;
	} else {
		list->bounds.height = height;
		list->bounds.width = width;
		list->bounds.x = x;
		list->bounds.y = y;

		list->start = (struct menu_entry *)malloc(sizeof(struct menu_entry));
		if (list->start == NULL) return NULL;
		list->end = list->start;
		list->start->prev = NULL;
		list->start->next = NULL;

		list->numEntries++;
		struct menu_entry *tmp;
		for (int i = 1; i < numEntries; i++) {
			struct menu_entry *tmp = (struct menu_entry *)malloc(sizeof(struct menu_entry));
			if (tmp == NULL) return NULL;
			printf("allocating entry at 0x%08X\r\n", tmp);
			list->end->next = tmp;
			tmp->prev = list->end;
			list->end = tmp;
			list->numEntries++;
		}

		// Now go back and fill in the graphics information for each entry
		update_menu_geometry(list, &list->bounds);
	return list;
	}
}

menu_list_t *new_menu_list_from(menu_info_t *info) {
	menu_list_t *tmp = new_menu_list(info->numEntries, info->bounds.height, info->bounds.width, info->bounds.x, info->bounds.y);
	if (tmp == NULL) return NULL;

	/* install flags */
	tmp->flags = info->flags;

	/* install colors */
	tmp->normal.bg = info->normal.bg;
	tmp->normal.fg = info->normal.fg;
	tmp->selected.bg = info->selected.bg;
	tmp->selected.fg = info->selected.fg;

	for (int i = 0; i < info->numEntries; i++) {
		struct menu_entry *tmp_entry = traverse_menu_list(tmp, i);
		tmp_entry->label = info->entry[i].label;
		tmp_entry->action = info->entry[i].action;
	}
	return tmp;
}

void update_menu_geometry(menu_list_t *list, struct gfx_box *box) {
	plot_t y_step = box->height / list->numEntries;
	for (int i = 0; i < list->numEntries; i++) {
		struct menu_entry *tmp = traverse_menu_list(list, i);
		tmp->bg.width = box->width;
		tmp->bg.x = box->x;
		tmp->bg.height = y_step;
		tmp->bg.y = box->y + (y_step * i);
	}
}

struct menu_entry *traverse_menu_list(menu_list_t *list, unsigned int index) {
	struct menu_entry *current = list->start;
	unsigned int current_index = 0;
	while (current_index < index) {
		if (current->next == NULL) return NULL;
		current = current->next;
		current_index++;
	}
	return current;
}

void inspect_menu_list(menu_list_t *list) {
	printf("menu: x=%d, y=%d, width=%d, height=%d\r\n", list->bounds.x, list->bounds.y, list->bounds.width, list->bounds.height);
	printf("number of entries: %d\r\n", list->numEntries);
	for (int i = 0; i < list->numEntries; i++) {
		struct menu_entry *tmp_entry = traverse_menu_list(list, i);
		printf("   entry %d:\r\n", i);
		printf("      label: \"%s\"\r\n", tmp_entry->label);
		printf("      action function: 0x%08X\r\n", tmp_entry->action);
	}
}

void free_menu_list(menu_list_t *menu) {

}

void draw_menu_list(menu_list_t *list, int selected_index) {
	for (int i = 0; i < list->numEntries; i++) {
		struct menu_entry *tmp_entry = traverse_menu_list(list, i);
		if (i == selected_index) {
			draw_gfx_box(&tmp_entry->bg, list->selected.fg, list->selected.bg);
			tui_set_color_fg_bg(list->selected.fg, list->selected.bg);
		} else {
			draw_gfx_box(&tmp_entry->bg, list->normal.fg, list->normal.bg);
			tui_set_color_fg_bg(list->normal.fg, list->normal.bg);
		}
		tui_cursor_setpos(tmp_entry->bg.x + (tmp_entry->bg.width - strlen(tmp_entry->label))/2, tmp_entry->bg.y + (tmp_entry->bg.height/2));
		printf(tmp_entry->label);
	}
	tui_reset_color_fg_bg();
}

void draw_gfx_box(struct gfx_box *box, unsigned char fg, unsigned char bg) {
	tui_set_color_fg_bg(fg, bg);
	for (int y = 0; y < box->height; y++) {
		tui_cursor_setpos(box->x, box->y + y);
		for (int x = 0; x < box->width; x++) {
			if (box->fill == NULL) putchar(' ');
			else putchar(box->fill);
		}
	}
	tui_reset_color_fg_bg();
}

/* returns the index from 1 of the entry under the point. returns 0 if no entry is under the point */
int get_entry_index_at_point(menu_list_t *list, point_t *point) {
	if (point->x < list->bounds.x+list->bounds.width && point->x >= list->bounds.x
		&& point->y < list->bounds.y+list->bounds.height && point->y >= list->bounds.y)
		return ((point->y - list->bounds.y) / (list->bounds.height/list->numEntries))+1;
	else
		return NULL;
}


action_t trigger_menu_list(menu_list_t *list) {
	int selected = 0;
	char tmpchr;
	point_t point;
	if (list->flags & ALLOW_MOUSE == ALLOW_MOUSE) {
		printf("\e[?9h\e[?1003h");
		tui_cursor_off();
	}
	while (true) {
		draw_menu_list(list, selected);
		tmpchr = getchar();
		if (tmpchr == '\033') { // if the first value is esc
    		getchar(); // skip the [
    		switch(getchar()) { // the real value
		        case 'A':
					if (selected > 0) selected--;
		            // code for arrow up
		            break;
		        case 'B':
					if (selected < list->numEntries-1) selected++;
		            // code for arrow down
		            break;
				case 'M':
					tmpchr = getchar();
					point.x = getchar()-32;
					point.y = getchar()-32;
					int tmp = get_entry_index_at_point(list, &point);
					if (tmp == 0) break;		// break if no valid entry is generated
					else selected = tmp - 1;

					/* if we register a click & the index is valid, fake an enter keypress to select the entry */
					if (tmpchr == 32) tmpchr = '\r';
					break;
    		}
		}
		if (tmpchr == '\r') {
			if (list->flags & ALLOW_MOUSE == ALLOW_MOUSE) {
				printf("\e[?9l\e[?1003l");
				tui_cursor_on();
			}
			return traverse_menu_list(list, selected)->action;
		}
	}
}

//---------------------------------------------------
