//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stdlib.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

#define CUTOFF 8

static void shortsort(char *lo, char *hi, unsigned width, int (*comp)(const void *, const void *));
static void swap(char *p, char *q, unsigned int width);

__weak_symbol void qsort(void *base, unsigned num, unsigned width, int (*comp)(const void *, const void *)) {
	char *lo, *hi;
	char *mid;
	char *l, *h;
	unsigned size;
	char *lostk[30], *histk[30];
	int stkptr;

	if (num < 2 || width == 0) return;
	stkptr = 0;

	lo = base;
	hi = (char *) base + width * (num - 1);

recurse:
	size = (hi - lo) / width + 1;

	if (size <= CUTOFF) {
		shortsort(lo, hi, width, comp);
	} else {
		mid = lo + (size / 2) * width;
		swap(mid, lo, width);

		l = lo;
		h = hi + width;

		while (1) {
			do { l += width; } while (l <= hi && comp(l, lo) <= 0);
			do { h -= width; } while (h > lo && comp(h, lo) >= 0);
			if (h < l) break;
			swap(l, h, width);
		}

		swap(lo, h, width);

		if (h - 1 - lo >= hi - l) {
			if (lo + width < h) {
				lostk[stkptr] = lo;
				histk[stkptr] = h - width;
				++stkptr;
			}

			if (l < hi) {
				lo = l;
				goto recurse;
			}
		} else {
			if (l < hi) {
				lostk[stkptr] = l;
				histk[stkptr] = hi;
				++stkptr;
			}

			if (lo + width < h) {
				hi = h - width;
				goto recurse;
			}
		}
	}

	--stkptr;
	if (stkptr >= 0) {
		lo = lostk[stkptr];
		hi = histk[stkptr];
		goto recurse;
	}
}

static void shortsort(char *lo, char *hi, unsigned width, int (*comp)(const void *, const void *)) {
	char *p, *max;

	while (hi > lo) {
		max = lo;
		for (p = lo + width; p <= hi; p += width) if (comp(p, max) > 0) max = p;
		swap(max, hi, width);
		hi -= width;
	}
}

static void swap(char *a, char *b, unsigned width) {
	char tmp;

	if (a != b) {
		while (width--) {
			tmp = *a;
			*a++ = *b;
			*b++ = tmp;
		}
	}
}
