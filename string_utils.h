#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include <cstdlib>
#include <cstring>
#include <cstdio>

#define BUFFER_INT 40
#define BUFFER_DOUBLE 300

char * to_str(int);
char * to_str(double);
char * to_str(const char *);

#endif