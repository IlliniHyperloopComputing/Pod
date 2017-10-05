#ifndef UTILS_H
#define UTILS_H
#include <cmath>
#include <algorithm>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
* Gets the current time since program startup
* @return a long long representing the number of microseconds since startup
**/
long long get_elapsed_time();

/**
* Wrapper for printf, prints only in debug mode
**/
void print_debug(const char * format, ...);

/**
* Wrapper for printf, prints only in test mode
**/
void print_test(const char * format, ...);

#endif // UTILS_H
