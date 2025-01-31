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
#include <stdlib.h>

typedef struct renderSettings_t
{
    uint16_t nthreads;
    uint16_t pixelsWide;
    uint16_t pixelsHigh;
    uint16_t samplesPerPixel;
    uint16_t maxDepth;
    char *outputPath;
} RenderSettings;

extern RenderSettings gRenderSettings;

// Sets render settings to sensible defaults.
void initRenderSettings(void);

void printRenderSettings(void);
