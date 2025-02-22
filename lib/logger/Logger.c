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

static LogLevel gThresholdLogLevel = LogLevelInfo;
static bool gSingleLineMode = false;

void SetThresholdLogLevel(LogLevel newThresholdLevel)
{
    gThresholdLogLevel = newThresholdLevel;
}

void SetSingleLineLogMode(bool singleLineMode)
{
    gSingleLineMode = singleLineMode;
}

void Logger(LogLevel level, const char *format, ...)
{
    if (level < gThresholdLogLevel) return;

    static char *nameForLevel[] = {"\033[33mDEBUG:\033[0m", "\033[32mINFO:\033[0m", "\033[35mWARNING:\033[0m",
                                   "\033[31mERROR:\033[0m", "\033[31mFAILED:\033[0m"};

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

    if (gSingleLineMode)
    {
        fprintf(stdout, "\r%s %s", nameForLevel[level], buffer);
        fflush(stdout);
    }
    else
    {
        fprintf(stdout, "%s %s\n", nameForLevel[level], buffer);
    }
}