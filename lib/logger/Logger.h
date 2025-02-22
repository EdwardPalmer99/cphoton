/**
 * @file Logger.h
 * @author Edward Palmer
 * @date 2025-02-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    LogLevelDebug = 0,
    LogLevelInfo,
    LogLevelWarning,
    LogLevelError,
    LogLevelFailed
} LogLevel;


void SetThresholdLogLevel(LogLevel level);
void SetSingleLineLogMode(bool singleLineMode);

// Writes log message to stdout in format: "[logLevel]: message"
void Logger(LogLevel level, const char *format, ...);

#define LogDebug(...) Logger(LogLevelDebug, __VA_ARGS__)
#define LogInfo(...) Logger(LogLevelInfo, __VA_ARGS__)
#define LogWarning(...) Logger(LogLevelWarning __VA_ARGS__)
#define LogError(...) Logger(LogLevelError, __VA_ARGS__)
#define LogFailed(...) Logger(LogLevelFailed __VA_ARGS__)
