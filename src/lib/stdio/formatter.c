//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------


#include "formatter.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "fpconv.h"

// ERRATA:
// %s and %c ignore 'l' specifier; all strings are char*
// %n is not and will likely never be supported

#define MAX_PADDING 32
static const char padding[MAX_PADDING] = "                                ";
static const char zeros[MAX_PADDING] = "00000000000000000000000000000000";

enum prefix {
	PREFIX_NONE = 0,
	PREFIX_PLUS = 1,
	PREFIX_MINUS = 2,
	PREFIX_SPACE = 3,
	PREFIX_OCTAL = 4,
	PREFIX_UPHEX = 5,
	PREFIX_LOWHEX = 6,
	PREFIX_0DOT = 7,
	PREFIX_PLUS_0DOT = 8,
	PREFIX_MINUS_0DOT = 9,
	PREFIX_SPACE_0DOT = 10,
};

static const struct format_chunk prefix_chunk[11] = {
	{"", 0}, // PREFIX_NONE
	{"+", 1}, // PREFIX_PLUS
	{"-", 1}, // PREFIX_MINUS
	{" ", 1}, // PREFIX_SPACE
	{"0", 1}, // PREFIX_OCTAL
	{"0X", 2}, // PREFIX_UPHEX
	{"0x", 2}, // PREFIX_LOWHEX
	{"0.", 2}, // PREFIX_0DOT
	{"+0.", 3}, // PREFIX_PLUS_0DOT
	{"-0.", 3}, // PREFIX_MINUS_0DOT
	{" 0.", 3}, // PREFIX_SPACE_0DOT
};

enum flags {
	FLAG_LEFT_JUSTIFY = 0x01,
	FLAG_PLUS_POSITIVE = 0x02,
	FLAG_SPACE_POSITIVE = 0x04,
	FLAG_ALTERNATE_FORM = 0x08,
	FLAG_PAD_WITH_ZERO = 0x10,
	FLAG_HAS_PRECISION = 0x20,
	FLAG_UPPERCASE = 0x40,
	FLAG_TRIM_ZEROS = 0x80,
};

struct spec {
	int minimum_width;
	int precision;
	int data_size;
	char flags;
	char conversion;
};

static uint64_t ulabs(int64_t n) { return (n < 0)? -n: n; }
static double dabs(double n) { return (n < 0.0)? -n: n; }

static void scan_literal(struct format_state *state) {
	state->body.addr = state->fmt;
	while (*state->fmt != '\0' && *state->fmt != '%') {
		state->fmt++;
	}
	state->body.size = state->fmt - state->body.addr;
}

static const char *parse(const char *fmt, va_list *arg, struct spec *spec) {
	// A format specifier has this syntax:
	// %[flags][width][.precision][length]specifier
	// This function parses the body of the specifier, so 'fmt' should point
	// at the first character following the leading '%'.
	// We will return the address of the character following the specifier.

	// Flags may come in any combination and any order. Left-justification
	// can apply to any conversion, but '+' and ' ' only apply to float and
	// signed-integer conversions, '#' only applies to octal, hex, and float
	// conversions, and '0' only applies to float conversions and to integer
	// conversions which have no specified precision.
	spec->flags = 0;
	do switch (*fmt++) {
		case '-': spec->flags |= FLAG_LEFT_JUSTIFY; continue;
		case '+': spec->flags |= FLAG_PLUS_POSITIVE; continue;
		case ' ': spec->flags |= FLAG_SPACE_POSITIVE; continue;
		case '#': spec->flags |= FLAG_ALTERNATE_FORM; continue;
		case '0': spec->flags |= FLAG_PAD_WITH_ZERO; continue;
		default: --fmt;
	} while(0);

	// Width is a series of digits or a single '*' character indicating that
	// the value should come from the argument list.
	spec->minimum_width = 0;
	if ('*' == *fmt) {
		spec->minimum_width = va_arg(*arg, int);
		if (spec->minimum_width < 0) {
			spec->flags |= FLAG_LEFT_JUSTIFY;
			spec->minimum_width *= -1;
		}
		fmt++;
	} else while (*fmt >= '0' && *fmt <= '9') {
		spec->minimum_width = spec->minimum_width * 10 + *fmt++ - '0';
	}

	// Precision begins with a '.' character if present; like the width, it
	// consists of a digit string or a '*' indicating that the value should
	// be read from the argument list.
	spec->precision = 0;
	if ('.' == *fmt) {
		spec->flags |= FLAG_HAS_PRECISION;
		if ('*' == *++fmt) {
			spec->precision = va_arg(*arg, int);
			if (spec->precision < 0) {
				spec->precision = 0;
				spec->flags &= ~FLAG_HAS_PRECISION;
			}
			fmt++;
		} else while (*fmt >= '0' && *fmt <= '9') {
			spec->precision = spec->precision * 10 + *fmt++ - '0';
		}
	}

	// Size, which is called "length" in the spec, specifies the size of the
	// input data relative to sizeof(int). We assume that signed and unsigned
	// types have the same size.
	spec->data_size = sizeof(int);
	switch (*fmt++) {
		case 'h': {
			spec->data_size = sizeof(short);
			if ('h' == *fmt) {
				fmt++;
				spec->data_size = sizeof(char);
			}
		} break;
		case 'l': {
			spec->data_size = sizeof(long);
			if ('l' == *fmt) {
				fmt++;
				spec->data_size = sizeof(long long);
			}
		} break;
		case 'j': spec->data_size = sizeof(intmax_t); break;
		case 'z': spec->data_size = sizeof(size_t); break;
		case 't': spec->data_size = sizeof(ptrdiff_t); break;
		default: --fmt;
	}

	// The last remaining character is the conversion specifier.
	spec->conversion = *fmt++;

	if (spec->conversion >= 'A' && spec->conversion <= 'Z') {
		spec->flags |= FLAG_UPPERCASE;
	}

	return fmt;
}

static void cvt_c(struct format_state *state, struct spec *spec, va_list *arg) {
	state->buffer[0] = va_arg(*arg, int);
	state->body.addr = state->buffer;
	state->body.size = 1;
}

static void cvt_s(struct format_state *state, struct spec *spec, va_list *arg) {
	const char *str = va_arg(*arg, const char*);
	state->body.addr = str;
	if (str) {
		if (spec->flags & FLAG_HAS_PRECISION) {
			const char *q = memchr(str, '\0', spec->precision);
			state->body.size = q? (q - str): spec->precision;
		} else {
			state->body.size = strlen(str);
		}
	} else {
		state->body.size = 0;
	}
}

static void sign_prefix(
		struct format_state *state, const struct spec *spec, bool negative) {
	if (negative) {
		state->prefix = PREFIX_MINUS;
	} else if (spec->flags & FLAG_PLUS_POSITIVE) {
		state->prefix = PREFIX_PLUS;
	} else if (spec->flags & FLAG_SPACE_POSITIVE) {
		state->prefix = PREFIX_SPACE;
	}
}

static void utoa(
		struct format_state *state,
		const struct spec *spec,
		uint64_t i,
		int radix) {
	// Render the integer 'i' into the format_state buffer, then point the
	// state's 'body' chunk at the section of buffer we used. We will write
	// the number from the end of the buffer back toward the beginning, since
	// that gives us most-to-least significant digit order, and leaves the
	// beginning of the buffer free for padding and/or prefixes if necessary.
	bool upper = spec->flags & FLAG_UPPERCASE;
	const char *digits = upper? "0123456789ABCDEF": "0123456789abcdef";
	char *buf = state->buffer + FORMAT_BUFFER_SIZE;
	bool has_precision = spec->flags & FLAG_HAS_PRECISION;
	if (i > 0 || !has_precision || spec->precision > 0) {
		do {
			*--buf = digits[i % radix];
			i /= radix;
		} while (i > 0);
	}
	state->body.addr = buf;
	state->body.size = FORMAT_BUFFER_SIZE - (buf - state->buffer);
	// If the spec requested a certain precision, make sure we have written
	// enough digits.
	if (has_precision && spec->precision > state->body.size) {
		state->leading_zeros = spec->precision - state->body.size;
	}
	// If the format includes a prefix, and there is room for it in the buffer,
	// copy it in so we don't have to return two separate chunks.
	if (state->prefix && 0 == spec->flags & FLAG_PAD_WITH_ZERO) {
		const struct format_chunk *prefix = &prefix_chunk[state->prefix];
		size_t combined_size = state->body.size + prefix->size;
		if (combined_size <= FORMAT_BUFFER_SIZE) {
			char *dest = state->buffer + FORMAT_BUFFER_SIZE - combined_size;
			memcpy(dest, prefix->addr, prefix->size);
			state->body.addr = dest;
			state->body.size = combined_size;
			state->prefix = PREFIX_NONE;
		}
	}
}

static int64_t iarg(size_t s, va_list *arg) {
	switch (s) {
		case 1: return (int8_t)va_arg(*arg, int);
		case 2: return (int16_t)va_arg(*arg, int);
		case 4: return va_arg(*arg, int32_t);
		case 8: return va_arg(*arg, int64_t);
		default: return va_arg(*arg, int);
	}
}

static uint64_t uarg(size_t s, va_list *arg) {
	switch (s) {
		case 1: return (uint8_t)va_arg(*arg, unsigned);
		case 2: return (uint16_t)va_arg(*arg, unsigned);
		case 4: return va_arg(*arg, uint32_t);
		case 8: return va_arg(*arg, uint64_t);
		default: return va_arg(*arg, unsigned);
	}
}

static void cvt_d(struct format_state *state, struct spec *spec, va_list *arg) {
	int64_t num = iarg(spec->data_size, arg);
	sign_prefix(state, spec, num < 0);
	utoa(state, spec, ulabs(num), 10);
}

static void cvt_x(struct format_state *state, struct spec *spec, va_list *arg) {
	bool upper = spec->flags & FLAG_UPPERCASE;
	if (spec->flags & FLAG_ALTERNATE_FORM) {
		state->prefix = upper? PREFIX_UPHEX: PREFIX_LOWHEX;
	} else {
		state->prefix = PREFIX_NONE;
	}
	uint64_t num = uarg(spec->data_size, arg);
	utoa(state, spec, num, 16);
}

static void cvt_o(struct format_state *state, struct spec *spec, va_list *arg) {
	bool alternate_form = spec->flags & FLAG_ALTERNATE_FORM;
	state->prefix = alternate_form? PREFIX_OCTAL: PREFIX_NONE;
	uint64_t num = uarg(spec->data_size, arg);
	utoa(state, spec, num, 8);
}

static void cvt_p(struct format_state *state, struct spec *spec, va_list *arg) {
	state->prefix = PREFIX_LOWHEX;
	uintptr_t num = va_arg(*arg, void*);
	utoa(state, spec, num, 16);
}

bool dtoa(struct format_state *state, struct spec *spec, double d, int *exp) {
	// Handle special values NAN, INF, and -INF, returning done (true).
	// Otherwise, use grisu2 to convert d into decimal, using the state
	// buffer. Pass the order value into parameter K and return false, since
	// the caller has more formatting work to do.
	union { double d; uint64_t u; } du;
	du.d = d;
	const uint64_t fracmask =  0x000FFFFFFFFFFFFFLLU;
	const uint64_t expmask =   0x7FF0000000000000LLU;
	const uint64_t signmask =  0x8000000000000000LLU;
	sign_prefix(state, spec, du.u & signmask);
	state->body.addr = state->buffer;
	bool done = true;
	if ((du.u & expmask) != expmask) {
		int K = 0;
		state->body.size = _fpconv_grisu2(d, state->buffer, &K);
		*exp = K + state->body.size - 1;
		done = false;
	} else {
		const char *msg = (du.u & fracmask)? "nanNAN": "infINF";
		if (spec->flags & FLAG_UPPERCASE) {
			msg += 3;
		}
		memcpy(state->buffer, msg, 3);
		state->body.size = 3;
	}
	return done;
}

static unsigned trim_zeros(const char *buf, unsigned size) {
	while (size > 0 && '0' == buf[size-1]) {
		--size;
	}
	if (size > 0 && '.' == buf[size-1]) {
		--size;
	}
	return size;
}

static bool set_sigfigs(struct format_state *state, int new_size) {
	bool shift = false;
	char *buf = state->buffer;
	int old_size = state->body.size;
	int diff = new_size - old_size;
	if (diff > 0) {
		memset(buf + old_size, '0', diff);
	}
	if (diff < 0) {
		char c = buf[new_size];
		bool carry = (c >= '5' && c <= '9');
		int index = new_size;
		while (carry) {
			if (index > 0) {
				--index;
			} else {
				memmove(buf+1, buf, new_size-1);
				*buf = '0';
				shift = true;
			}
			c = buf[index];
			carry = (c == '9');
			buf[index] = carry? '0': c+1;
		}
	}
	state->body.size = new_size;
	return shift;
}

static void emit_e(struct format_state *state, struct spec *spec, int exp) {
	char *buf = state->buffer;
	int size = state->body.size;
	// Insert a decimal point after the first digit.
	if (size > 1 || spec->flags & FLAG_ALTERNATE_FORM) {
		memmove(buf + 2, buf + 1, size - 1);
		buf[1] = '.';
		++size;
	}
	if (spec->flags & FLAG_TRIM_ZEROS) {
		size = trim_zeros(buf, size);
	}
	// Suffix with the exponent, which must always be at least 2 digits
	// and may never be more than 4 digits long. From the standard:
	// "The exponent always contains at least two digits, and only as many
	// more digits as necessary to represent the exponent."
	bool upper = spec->flags & FLAG_UPPERCASE;
	buf[size++] = upper? 'E': 'e';
	buf[size++] = (exp >= 0)? '+': '-';
	exp = ulabs(exp);
	if (exp >= 1000) {
		buf[size++] = (exp / 1000) % 10 + '0';
	}
	if (exp >= 100) {
		buf[size++] = (exp / 100) % 10 + '0';
	}
	buf[size++] = (exp / 10) % 10 + '0';
	buf[size++] = (exp / 1) % 10 + '0';
	state->body.size = size;
}

static void cvt_e(struct format_state *state, struct spec *spec, va_list *arg) {
	double num = va_arg(*arg, double);
	int exp = 0;
	if (dtoa(state, spec, num, &exp)) return;
	int precision = spec->flags & FLAG_HAS_PRECISION? spec->precision: 6;
	if (set_sigfigs(state, precision + 1)) {
		++exp;
	}
	emit_e(state, spec, exp);
}

static void emit_f(struct format_state *state, struct spec *spec, int exp, int precision) {
	char *buf = state->buffer;
	int size = state->body.size;
	// K represents the location of the decimal point relative to the end of
	// the string we just generated.
	if (exp < 0) {
		// The number is smaller than 1, so it leads off with some "0."
		// prefix, depending on sign options.
		switch (state->prefix) {
			case PREFIX_NONE: state->prefix = PREFIX_0DOT; break;
			case PREFIX_MINUS: state->prefix = PREFIX_MINUS_0DOT; break;
			case PREFIX_PLUS: state->prefix = PREFIX_PLUS_0DOT; break;
			case PREFIX_SPACE: state->prefix = PREFIX_SPACE_0DOT; break;
			default: state->prefix = PREFIX_0DOT;
		}
		// How many leading zeros do we need?
		state->leading_zeros = exp + 1;
		if (state->leading_zeros < precision) {
			// If there's room for more digits, use the ones we generated.
			int available = precision - state->leading_zeros;
			if (size > available) {
				size = available;
			}
		} else {
			// Don't generate more zeros than we actually need to show.
			state->leading_zeros = precision;
			size = 0;
		}
		if (spec->flags & FLAG_TRIM_ZEROS) {
			size = trim_zeros(buf, size);
		}
	} else if (exp < size) {
		// The decimal point should be located somewhere within the string.
		// Move the characters, then either round or pad to make the number
		// of digits following the decimal equal to the precision requested.
		int leading = exp + 1;
		char *pos = buf + leading;
		memmove(pos + 1, pos, size - leading);
		*pos = '.';
		++size;
		if (spec->flags & FLAG_TRIM_ZEROS) {
			size = trim_zeros(buf, size);
		}
	} else {
		// The decimal point appears somewhere to the right of the string.
		// This implies that all the fractional digits are also zeros.
		state->trailing_zeros = exp - size + 1;
		if (0 == (spec->flags & FLAG_TRIM_ZEROS)) {
			state->trailing_point = true;
			state->trailing_fraction = precision;
		}
	}
	state->body.size = size;
}

static void cvt_f(struct format_state *state, struct spec *spec, va_list *arg) {
	double num = va_arg(*arg, double);
	int exp = 0;
	if (dtoa(state, spec, num, &exp)) return;
	int precision = spec->flags & FLAG_HAS_PRECISION? spec->precision: 6;
	while (set_sigfigs(state, precision + exp + 1)) {
		++exp;
	}
	emit_f(state, spec, exp, precision);
}

static void cvt_g(struct format_state *state, struct spec *spec, va_list *arg) {
	double num = va_arg(*arg, double);
	int exp = 0;
	if (dtoa(state, spec, num, &exp)) return;
	int precision = spec->flags & FLAG_HAS_PRECISION? spec->precision: 6;
	if (precision < 1) {
		precision = 1;
	}
	if (set_sigfigs(state, precision)) {
		++exp;
	}
	// With mode %g, precision controls the total number of significant digits,
	// not (as with %e and %f) the number of digits after the decimal point.
	// We'll print very large or small numbers using scientific notation, while
	// numbers closer to zero will be printed as simple decimals.
	spec->flags |= FLAG_TRIM_ZEROS;
	if (exp < -4 || exp >= precision) {
		emit_e(state, spec, exp);
	} else {
		emit_f(state, spec, exp, precision);
	}
}

static void cvt_a(struct format_state *state, struct spec *spec, va_list *arg) {
	union {
		double d;
		uint64_t u;
	} num;
	num.d = va_arg(*arg, double);
	sign_prefix(state, spec, num.d < 0.0);
	bool upper = spec->flags & FLAG_UPPERCASE;
	char *buf = state->buffer;
	*buf++ = '0';
	*buf++ = upper? 'X': 'x';
	// The first digit is 1 for normalized, 0 for denormalized. Most numbers
	// will be normalized.
	const uint64_t expmask =   0x7FF0000000000000LLU;
	bool normal = (num.u & expmask);
	*buf++ = normal? '1': '0';
	*buf++ = '.';
	// Print the fractional part of the value out as an ordinary hex literal,
	// with as many digits as the requested precision, up to the total number
	// of digits available.
	int precision = spec->flags & FLAG_HAS_PRECISION? spec->precision: 6;
	const char *alpha = upper? "0123456789ABCDEF": "0123456789abcdef";
	const uint64_t fracmask =  0x000FFFFFFFFFFFFFLLU;
	uint64_t frac = num.u & fracmask;
	for (unsigned i = 0; i < 12 && i < precision; ++i) {
		int val = (frac >> ((12 - i)*4)) & 0x0F;
		*buf++ = alpha[val];
	}
	// Trim off any trailing zeros.
	while (buf[-1] == '0') {
		--buf;
	}
	*buf++ = upper? 'P': 'p';
	// Print the exponent as a one, two, or three-digit decimal.
	int exp = ((num.u & expmask) >> 52) - 1023;
	*buf++ = (exp < 0)? '-': '+';
	exp = ulabs(exp);
	if (exp >= 100) *buf++ = alpha[(exp / 100) % 10];
	if (exp >= 10) *buf++ = alpha[(exp / 10) % 10];
	*buf++ = alpha[exp % 10];
	state->body.addr = state->buffer;
	state->body.size = buf - state->buffer;
}

static void convert(struct format_state *state, va_list *arg) {
	// Skip the leading % character and parse the specifier body.
	struct spec spec = {0};
	state->fmt = parse(++state->fmt, arg, &spec);

	// Zero-padding only applies to right-justified conversions; if the '-'
	// flag was preset, we'll ignore '0', whichever order they occurred in.
	if (spec.flags & FLAG_LEFT_JUSTIFY) {
		spec.flags &= ~FLAG_PAD_WITH_ZERO;
	}
	// Using a space for positive numbers only applies if we are not already
	// using a plus sign; if the '+' flag was present, ignore ' '.
	if (spec.flags & FLAG_PLUS_POSITIVE) {
		spec.flags &= ~FLAG_SPACE_POSITIVE;
	}

	switch (spec.conversion) {
		case 'c': cvt_c(state, &spec, arg); break;
		case 's': cvt_s(state, &spec, arg); break;
		case 'i':
		case 'd': cvt_d(state, &spec, arg); break;
		case 'x':
		case 'X': cvt_x(state, &spec, arg); break;
		case 'o': cvt_o(state, &spec, arg); break;
		case 'p': cvt_p(state, &spec, arg); break;
		case 'g':
		case 'G': cvt_g(state, &spec, arg); break;
		case 'e':
		case 'E': cvt_e(state, &spec, arg); break;
		case 'f':
		case 'F': cvt_f(state, &spec, arg); break;
		case 'a':
		case 'A': cvt_a(state, &spec, arg); break;
		case '%':
		default: {
			state->buffer[0] = spec.conversion;
			state->body.addr = state->buffer;
			state->body.size = 1;
		} break;
	}

	// Combining the three possible body elements, how many characters did
	// we just generate?
	size_t width = 0;
	width += state->leading_spaces;
	width += prefix_chunk[state->prefix].size;
	width += state->leading_zeros;
	width += state->body.size;
	width += state->trailing_spaces;

	// If the text we've generated for this specifier is smaller than the
	// minimum field width, pad it out with spaces.
	int minimum_width = spec.minimum_width;
	if (minimum_width > width) {
		int padding = minimum_width - width;
		bool left_justify = spec.flags & FLAG_LEFT_JUSTIFY;
		bool pad_with_zero = spec.flags & FLAG_PAD_WITH_ZERO;
		if (left_justify) {
			state->trailing_spaces += padding;
		} else if (pad_with_zero) {
			state->leading_zeros += padding;
		} else {
			state->leading_spaces += padding;
		}
	}
}

void _format_start(struct format_state *state, const char *format_string) {
	memset(state, 0, sizeof(struct format_state));
	state->fmt = format_string;
}

static bool outpad(struct format_state *state, const char *src, size_t *len) {
	if (0 == *len) return false;
	state->current.addr = src;
	state->current.size = (*len <= MAX_PADDING)? *len: MAX_PADDING;
	*len -= state->current.size;
	return true;
}

static bool outchunk(struct format_state *state, const struct format_chunk *chk) {
	if (0 == chk->size) return false;
	state->current = *chk;
	return true;
}

bool _format_next(struct format_state *state, va_list *arg) {
	while (1) {
		if (outpad(state, padding, &state->leading_spaces)) return true;
		if (outchunk(state, &prefix_chunk[state->prefix])) {
			state->prefix = PREFIX_NONE;
			return true;
		}
		if (outpad(state, zeros, &state->leading_zeros)) return true;
		if (outchunk(state, &state->body)) {
			state->body.size = 0;
			return true;
		}
		if (outpad(state, zeros, &state->trailing_zeros)) return true;
		if (state->trailing_point) {
			state->current.addr = ".";
			state->current.size = 1;
			state->trailing_point = false;
			return true;
		}
		if (outpad(state, zeros, &state->trailing_fraction)) return true;
		if (outpad(state, padding, &state->trailing_spaces)) return true;
		switch (*state->fmt) {
			case '%': convert(state, arg); break;
			case '\0': return false;
			default: scan_literal(state); break;
		}
	}
	return false;
}
