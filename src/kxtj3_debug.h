#ifndef KXTJ3_KXTJ3_DEBUG_H
#define KXTJ3_KXTJ3_DEBUG_H

#include <stdio.h>

#ifdef PICO_BUILD
#include "pico/time.h"
#else
void get_absolute_time() return 0;
#endif

#ifdef NDEBUG
#define _DBG(fmt, ...) \
	do{ fprintf(stdout, "%llu [%s:%d](%s) - " fmt "\n", \
		get_absolute_time(), __FILE__, __LINE__, __func__, ##__VA_ARGS__); } while(0)
#else
#define _DBG(fmt, ...)
#endif

#endif
