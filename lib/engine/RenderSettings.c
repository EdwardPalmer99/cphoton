/**
 * @file RenderSettings.c
 * @author Edward Palmer
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "engine/RenderSettings.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off
RenderSettings gRenderSettings = {.nthreads = 8,
                                  .pixelsWide = 2560,
                                  .pixelsHigh = 1600,
                                  .samplesPerPixel = 100,
                                  .maxDepth = 50,
                                  .outputPath = NULL};
// clang-format on

void printCLIOptions(const char *programName);


void parseCLIOptions(int argc, const char *argv[])
{
    // NB: skip first argument which is program name.
    const int kBufferSize = 1024;

    char argBuffer[kBufferSize];

    bool hasRequiredArg = false;

    for (int iarg = 1; iarg != argc; ++iarg)
    {
        strncpy(argBuffer, argv[iarg], kBufferSize - 1);
        argBuffer[kBufferSize - 1] = '\0';

        char *name = NULL;
        char *value = NULL;

        for (char *c = (char *)argBuffer; *c; ++c)
        {
            if (*c == '=')
            {
                *c = '\0';

                name = argBuffer;
                value = (c + 1);
                break;
            }
        }

        if (!name || !value)
        {
            if (strcmp(argBuffer, "--help") == 0)
            {
                printCLIOptions(argv[0]);
                exit(EXIT_SUCCESS);
            }
            else
            {
                printCLIOptions(argv[0]);
                exit(EXIT_FAILURE);
            }
        }

        if (strcmp(name, "--path") == 0)
        {
            gRenderSettings.outputPath = strdup((char *)value);
            hasRequiredArg = true;
        }
        else
        {
            int outputValue = atoi(value);

            if (outputValue < 1 || outputValue > 10000)
            {
                fprintf(stderr, "error: invalid value: %s for argument: %s\n", value, name);
                exit(EXIT_FAILURE);
            }

            uint16_t unsignedValue = (uint16_t)outputValue;

            if (strcmp(name, "--nthreads") == 0)
                gRenderSettings.nthreads = unsignedValue;
            else if (strcmp(name, "--width") == 0)
                gRenderSettings.pixelsWide = unsignedValue;
            else if (strcmp(name, "--height") == 0)
                gRenderSettings.pixelsHigh = unsignedValue;
            else if (strcmp(name, "--samples-per-pixel") == 0)
                gRenderSettings.samplesPerPixel = unsignedValue;
            else if (strcmp(name, "--max-depth") == 0)
                gRenderSettings.maxDepth = unsignedValue;
        }
    }

    if (!hasRequiredArg)
    {
        printCLIOptions(argv[0]);
        exit(EXIT_FAILURE);
    }
}


void printCLIOptions(const char *programName)
{
    fprintf(stdout,
            "Usage: %s [option(s)] --path=[render path]\n"
            " Render the scene\n"
            " The options are:\n"
            "  --path              path for output render\n"
            "  --help              print this message and exit\n"
            "  --nthreads          number of threads to use in a render (default: %u)\n"
            "  --width             image output width in pixels (default: %u)\n"
            "  --height            image output height in pixels (default: %u)\n"
            "  --samples-per-pixel number of rays averaged over for a pixel (default: %u)\n"
            "  --max-depth         maximum number of bounces of a ray (default: %u)\n",
            programName, gRenderSettings.nthreads, gRenderSettings.pixelsWide, gRenderSettings.pixelsHigh,
            gRenderSettings.samplesPerPixel, gRenderSettings.maxDepth);
}
