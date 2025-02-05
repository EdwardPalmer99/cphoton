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

// Writes log message to stdout in format: "YYYY-MM-DD HH:MM:SS logLevel [func] [file:line] message"
void PrintLogMessage(LogLevel level, const char *func, const char *file, unsigned int line, const char *format, ...);

#define Logger(logLevel, ...)                                                                                          \
    {                                                                                                                  \
        PrintLogMessage((logLevel), __func__, __FILE__, __LINE__, __VA_ARGS__);                                        \
    }
