#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

// Stores the trimmed input string into the given output buffer, which must be
// large enough to store the result.  If it is too small, the output is
// truncated. Copied pretty much vertabim from http://stackoverflow.com/a/122721
int trimwhitespace
(char *out, int len, const char *str)
{
	if(len == 0) {
		return 0;
	}

	const char *end;
	int out_size;

	// Trim leading space
	while(isspace(*str)) str++;

	// All spaces?
	if(*str == 0) {
		*out = 0;
		return 1;
	}

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;
	end++;

	// Set output size to minimum of trimmed string length and buffer size minus 1
	out_size = (end - str) < len-1 ? (end - str) : len-1;

	// Copy trimmed string and add null terminator
	memcpy(out, str, out_size);
	out[out_size] = 0;
	return out_size;
}
