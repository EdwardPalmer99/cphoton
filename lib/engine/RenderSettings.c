/**
 * @file RenderSettings.c
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "RenderSettings.h"
#include <stdio.h>

RenderSettings gRenderSettings;

void initRenderSettings(void)
{
    gRenderSettings.nthreads = 8;
    gRenderSettings.samplesPerPixel = 100;
    gRenderSettings.maxDepth = 50;
    gRenderSettings.pixelsWide = 2560;
    gRenderSettings.pixelsHigh = (gRenderSettings.pixelsWide / 1.6);
    gRenderSettings.outputPath = NULL;
}


void printRenderSettings(void)
{
    printf("=== Render Settings ===\n"
           "nthreads: %d\n"
           "path: %s\n"
           "width: %d\n"
           "height: %d\n"
           "samples-per-pixel: %d\n"
           "max-depth: %d\n"
           "=======================\n",
           gRenderSettings.nthreads,
           gRenderSettings.outputPath,
           gRenderSettings.pixelsWide,
           gRenderSettings.pixelsHigh,
           gRenderSettings.samplesPerPixel,
           gRenderSettings.maxDepth);
}
