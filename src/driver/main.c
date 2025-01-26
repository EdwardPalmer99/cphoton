//
//  main.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "DemoScenes.h"
#include "RenderSettings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    // Crude parsing of CLI arguments:
    initRenderSettings();

    for (int iarg = 1; iarg < (argc - 1); iarg += 2)
    {
        const char *arg = argv[iarg];
        const char *next = argv[iarg + 1];

        if (strcmp(arg, "--nthreads") == 0)
        {
            gRenderSettings.nthreads = atoi(next); // Dangerous!
        }
        else if (strcmp(arg, "--path") == 0)
        {
            gRenderSettings.outputPath = (char *)next; // No need to copy.
        }
        else if (strcmp(arg, "--width") == 0 || strcmp(arg, "-w") == 0)
        {
            gRenderSettings.pixelsWide = atoi(next);
        }
        else if (strcmp(arg, "--height") == 0 || strcmp(arg, "-h") == 0)
        {
            gRenderSettings.pixelsHigh = atoi(next);
        }
        else if (strcmp(arg, "--samples-per-pixel") == 0)
        {
            gRenderSettings.samplesPerPixel = atoi(next);
        }
        else if (strcmp(arg, "--max-depth") == 0)
        {
            gRenderSettings.maxDepth = atoi(next);
        }
        else
        {
            fprintf(stderr, "error: unexpected argument: %s\n", arg);
            return EXIT_FAILURE;
        }
    }

    // TODO: - add additional verification checks.

    if (!gRenderSettings.outputPath)
    {
        fprintf(stderr, "error: no output path supplied\n");
        return EXIT_FAILURE;
    }

    printRenderSettings();
    renderBatCave();
    return 0;
}
