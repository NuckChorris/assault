#pragma once

#define CENTERED(outer,inner) (((outer) / 2.0) - ((inner) / 2.0))
#define UNUSED __attribute__((__unused__))

int trimwhitespace (char *out, int len, const char *str);
