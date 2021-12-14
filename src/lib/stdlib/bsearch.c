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

__weak_symbol void *bsearch(const void *key, const void *base, size_t num, size_t width, __compare fn) {
	char *lo = (char *) base;
	char *hi = (char *) base + (num - 1) * width;
	char *mid;
	unsigned int half;
	int result;

	while (lo <= hi) {
		if (half = num / 2) {
			mid = lo + (num & 1 ? half : (half - 1)) * width;
			if (!(result = fn(key,mid))) {
				return mid;
			} else if (result < 0) {
				hi = mid - width;
				num = num & 1 ? half : half - 1;
			} else {
				lo = mid + width;
				num = half;
			}
		} else if (num) {
			return (fn(key, lo) ? NULL : lo);
		} else {
			break;
		}
	}
	return NULL;
}
