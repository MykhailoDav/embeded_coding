#pragma once

#include <stdio.h>
#include <stdlib.h>

#if defined(DEBUG)
void printlog(const char *fmt, ...);
#define LOG_DEBUG(tg, fmt,...) \
printlog("D [%s] (%s:%d) " fmt "\n", tg, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define LOG_DEBUG(tg, fmt,...)
#endif