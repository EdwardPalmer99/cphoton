/**
 * @file RenderSettings.h
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <stdint.h>

typedef struct renderSettings_t
{
    uint16_t pixelsWide;
    uint16_t pixelsHigh;
    char *outputPath;
} RenderSettings;

extern RenderSettings gRenderSettings;

void parseCLIOptions(int argc, const char *argv[]);
