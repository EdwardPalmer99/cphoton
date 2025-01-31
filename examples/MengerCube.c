/**
 * @file MengerCube.c
 * @author Edward Palmer
 * @date 2025-01-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <cphoton/Camera.h>
#include <cphoton/Material.h>
#include <cphoton/MengerCube.h>
#include <cphoton/PPMWriter.h>
#include <cphoton/RayTracer.h>
#include <cphoton/RenderSettings.h>
#include <cphoton/Scene.h>
#include <cphoton/Texture.h>

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
    // Works well with 4/3 aspect ratio, width=800.
    const double aspectRatio = ((double)gRenderSettings.pixelsWide / (double)gRenderSettings.pixelsHigh);

    Camera camera = makeCamera(45.0, aspectRatio, 4, 0.0, point3(2, 5, 5), point3(0.2, 0.6, 1.0));

    // Create the textures:
    Texture *greyColor = makeSolidTexture(color3(0.20, 0.26, 0.35));
    Texture *goldColor = makeSolidTexture(makeGoldColor());

    // Create the materials:
    Material *greyMetal = makeMetal(greyColor, 0.2);
    Material *goldLambertian = makeLambertian(goldColor);

    // Create the scene:
    Scene *scene = makeScene();

    Primitive *mengerSponge0 = makeMengerSponge(0, point3(-1.5, 0.5, -1.5), 1.0, goldLambertian);
    Primitive *mengerSponge1 = makeMengerSponge(1, point3(1.5, 0.5, -1.5), 1.0, goldLambertian);
    Primitive *mengerSponge3 = makeMengerSponge(3, point3(-1.5, 0.5, 1.5), 1.0, goldLambertian);
    Primitive *mengerSponge2 = makeMengerSponge(2, point3(1.5, 0.5, 1.5), 1.0, goldLambertian);
    Primitive *mengerSponge4 = makeMengerSponge(4, point3(0, 0.5, 0), 1.0, goldLambertian);
    Primitive *plane = makePlane(point3(0, 0, 0), vector3(0, 1, 0), greyMetal);

    scene->addObject(scene, mengerSponge0);
    scene->addObject(scene, mengerSponge1);
    scene->addObject(scene, mengerSponge2);
    scene->addObject(scene, mengerSponge3);
    scene->addObject(scene, mengerSponge4);
    scene->addObject(scene, plane);
    scene->markAsFinished(scene);

    // Render:
    PPMImage *outputImage = renderScene(scene, &camera);
    writeBinary16BitPPMImage(outputImage, gRenderSettings.outputPath);

    // Cleanup:
    freePPMImage(outputImage);
    scene->destructor(scene);

    return 0;
}
