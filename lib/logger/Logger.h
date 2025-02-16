/**
 * @file Logger.h
 * @author Edward Palmer
 * @date 2025-02-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    LoggerDebug = 0,
    LoggerInfo,
    LoggerWarn,
    LoggerError,
    LoggerCritical
} LogLevel;

// Writes log message to stdout in format: "YYYY-MM-DD HH:MM:SS.000000 [logLevel] [file:line] message"
void PrintLogMessage(LogLevel level, const char *file, unsigned int line, const char *format, ...);

#define Logger(logLevel, ...)                                                                                          \
    {                                                                                                                  \
        PrintLogMessage((logLevel), __FILE__, __LINE__, __VA_ARGS__);                                                  \
    }
