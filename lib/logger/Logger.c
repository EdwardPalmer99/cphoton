/**
 * @file Logger.c
 * @author Edward Palmer
 * @date 2025-02-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Logger.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void PrintLogMessage(LogLevel level, const char *file, unsigned int line, const char *format, ...)
{
    if (!file || (level < LoggerDebug || level > LoggerCritical))
    {
        fprintf(stderr, "error: invalid arguments passed to %s\n", __func__);
        fflush(stdout);
        abort();
    }

    static char *nameForLevel[] = {"\033[37mDEBUG\033[0m", "\033[32mINFO\033[0m", "\033[33mWARN\033[0m",
                                   "\033[31mERROR\033[0m", "\033[35mCRITICAL\033[0m"};

    const int kBufferSize = 256;

    char buffer[kBufferSize];

    va_list args;
    va_start(args, format);
    int nwritten = vsnprintf(buffer, kBufferSize, format, args);
    va_end(args);

    bool ok = (nwritten > 0 && nwritten < kBufferSize);
    if (!ok)
    {
        fprintf(stderr, "error: failed to write to log buffer\n");
        fflush(stdout);
        abort();
    }

    // Get current time:
    time_t now = time(NULL);
    struct tm *gmtNow = gmtime(&now);

    char timeBuffer[20];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", gmtNow);

    fprintf(stdout, "%s %s [%s:%u] %s\n", timeBuffer, nameForLevel[level], file, line, buffer);
}