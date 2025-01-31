/**
 * @file BatCave.c
 * @author Edward Palmer
 * @date 2025-01-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <cphoton/Camera.h>
#include <cphoton/PPMWriter.h>
#include <cphoton/Primitive.h>
#include <cphoton/RayTracer.h>
#include <cphoton/RenderSettings.h>
#include <cphoton/Scene.h>
#include <cphoton/Texture.h>
#include <cphoton/Vector3.h>


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

    // Create the camera:
    const double aspectRatio = ((double)gRenderSettings.pixelsWide / (double)gRenderSettings.pixelsHigh);
    Camera camera = makeCamera(45.0, aspectRatio, 1, 0, point3(-2.5, 2, 10), point3(0, 2, 0));

    Primitive *room = makeDarkKnightRoom(20, 16.0, 5);

    Scene *scene = makeScene();

    scene->addObject(scene, room);

    // Monolith:
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            Primitive *cube = makeCube(point3(0.5 * i, 0.25 + 0.5 * j, 0), zeroVector(), 0.5, makeLambertian(makeSolidTexture(color3(.01, .01, .01))));

            scene->addObject(scene, cube);
        }
    }

    scene->markAsFinished(scene);

    PPMImage *outputImage = renderScene(scene, &camera);

    writeBinary16BitPPMImage(outputImage, gRenderSettings.outputPath);

    scene->destructor(scene);
    freePPMImage(outputImage);

    return 0;
}
