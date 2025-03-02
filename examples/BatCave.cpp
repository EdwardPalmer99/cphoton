/**
 * @file BatCave.c
 * @author Edward Palmer
 * @date 2025-01-31
 *
 * @copyright Copyright (c) 2025
 *
 */

extern "C"
{
#include "CPhoton.h"
}

#include "engine/PhotonEngine.hpp"
#include <cstdlib>

Primitive *makeDarkKnightRoom(double length, double width, double height);

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
            Primitive *cube = makeCube(point3(0.5 * i, 0.25 + 0.5 * j, 0), zeroVector(), 0.5,
                                       makeLambertian(makeSolidTexture(color3(.01, .01, .01))));

            scene->addObject(scene, cube);
        }
    }

    scene->markAsFinished(scene);

    PhotonEngine engine(gRenderSettings.pixelsWide, gRenderSettings.pixelsHigh);

    PPMImage *outputImage = engine.render(scene, &camera);

    writeBinary16BitPPMImage(outputImage, gRenderSettings.outputPath);

    scene->destructor(scene);
    freePPMImage(outputImage);

    return 0;
}


Primitive *makeDarkKnightRoom(double length, double width, double height)
{
    const double halfRoomW = 0.5 * width;
    const double halfRoomL = 0.5 * length;

    Material *wallMaterial = makeLambertian(makeSolidTexture(color3(0.05, 0.05, 0.05)));
    Material *lightMaterial = makeEmitter(color3(.9, .9, .9));

    int numObjects = 0;
    int objectCapacity = 100;

    Primitive **objects = (Primitive **)malloc(sizeof(Primitive *) * objectCapacity);
    if (!objects) return NULL;

    objects[numObjects++] = makePlane(point3(halfRoomW, 0, 0), vector3(-1, 0, 0), wallMaterial);
    objects[numObjects++] = makePlane(point3(-halfRoomW, 0, 0), vector3(1, 0, 0), wallMaterial);
    objects[numObjects++] = makePlane(point3(0, 0, -halfRoomL), vector3(0, 0, 1), wallMaterial);
    objects[numObjects++] = makePlane(point3(0, 0, halfRoomL), vector3(0, 0, -1), wallMaterial);
    objects[numObjects++] = makePlane(point3(0, height, 0), vector3(0, -1, 0), wallMaterial);
    objects[numObjects++] = makePlane(point3(0, 0, 0), vector3(0, 1, 0), wallMaterial);

    // Create all of the lights on the ceiling:
    for (int i = -halfRoomW; i <= halfRoomW; i += 1)
    {
        for (int j = -halfRoomL; j <= halfRoomL; j += 1)
        {
            Primitive *floorPanel = makeCube(point3(i + 0.495, -0.490, j + 0.495), zeroVector(), 0.99, wallMaterial);
            Primitive *ceilingPanel =
                makeCube(point3(i + 0.495, height + .494, j + 0.495), zeroVector(), 0.99, lightMaterial);

            if (numObjects > 0.8 * objectCapacity)
            {
                void *reallocPtr = realloc(objects, sizeof(Primitive *) * objectCapacity * 2);
                if (!reallocPtr)
                {
                    free(objects);
                    return NULL;
                }

                objects = (Primitive **)reallocPtr;
                objectCapacity *= 2;
            }

            objects[numObjects++] = floorPanel;
            objects[numObjects++] = ceilingPanel;
        }
    }

    Primitive *roomNode = makeBVHNode(objects, 0, numObjects);

    free(objects);
    return roomNode;
}
