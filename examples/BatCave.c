/**
 * @file BatCave.c
 * @author Edward Palmer
 * @date 2025-01-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Camera.h"
#include "PPMWriter.h"
#include "Primitive.h"
#include "RayTracer.h"
#include "RenderSettings.h"
#include "Scene.h"
#include "Texture.h"
#include "Vector3.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, const char *argv[])
{
    parseCLIOptions(argc, argv);

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
