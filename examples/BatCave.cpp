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
#include "engine/RenderSettings.h"
#include "utility/PPMWriter.h"
}

#include "engine/PhotonEngine.hpp"
#include "engine/Scene.hpp"
#include "engine/primitives/BVHNode.hpp"
#include "engine/primitives/Cube.hpp"
#include "engine/primitives/Plane.hpp"
#include "engine/primitives/Primitive.hpp"

#include <cstdlib>
#include <vector>

Primitive *makeDarkKnightRoom(double length, double width, double height);

int main(int argc, const char *argv[])
{
    parseCLIOptions(argc, argv);

    // Create the camera:
    const double aspectRatio = ((double)gRenderSettings.pixelsWide / (double)gRenderSettings.pixelsHigh);
    Camera camera(45.0, aspectRatio, 1, 0, point3(-2.5, 2, 10), point3(0, 2, 0));

    Primitive *room = makeDarkKnightRoom(20, 16.0, 5);

    Scene scene;
    scene.addObject(room);

    // Monolith:
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            Primitive *cube = new Cube(point3(0.5 * i, 0.25 + 0.5 * j, 0), zeroVector(), 0.5,
                                       makeLambertian(makeSolidTexture(color3(.01, .01, .01))));

            scene.addObject(cube);
        }
    }

    PhotonEngine engine(gRenderSettings.pixelsWide, gRenderSettings.pixelsHigh);

    PPMImage *outputImage = engine.render(&scene, &camera);

    writeBinary16BitPPMImage(outputImage, gRenderSettings.outputPath);

    freePPMImage(outputImage);

    return 0;
}


Primitive *makeDarkKnightRoom(double length, double width, double height)
{
    const double halfRoomW = 0.5 * width;
    const double halfRoomL = 0.5 * length;

    Material *wallMaterial = makeLambertian(makeSolidTexture(color3(0.05, 0.05, 0.05)));
    Material *lightMaterial = makeEmitter(color3(.9, .9, .9));

    std::vector<Primitive *> objects;

    objects.push_back(new Plane(point3(halfRoomW, 0, 0), vector3(-1, 0, 0), wallMaterial));
    objects.push_back(new Plane(point3(-halfRoomW, 0, 0), vector3(1, 0, 0), wallMaterial));
    objects.push_back(new Plane(point3(0, 0, -halfRoomL), vector3(0, 0, 1), wallMaterial));
    objects.push_back(new Plane(point3(0, 0, halfRoomL), vector3(0, 0, -1), wallMaterial));
    objects.push_back(new Plane(point3(0, height, 0), vector3(0, -1, 0), wallMaterial));
    objects.push_back(new Plane(point3(0, 0, 0), vector3(0, 1, 0), wallMaterial));

    // Create all of the lights on the ceiling:
    for (int i = -halfRoomW; i <= halfRoomW; i += 1)
    {
        for (int j = -halfRoomL; j <= halfRoomL; j += 1)
        {
            Primitive *floorPanel = new Cube(point3(i + 0.495, -0.490, j + 0.495), zeroVector(), 0.99, wallMaterial);

            Primitive *ceilingPanel =
                new Cube(point3(i + 0.495, height + .494, j + 0.495), zeroVector(), 0.99, lightMaterial);

            objects.push_back(floorPanel);
            objects.push_back(ceilingPanel);
        }
    }

    objects.shrink_to_fit();
    return new BVHNode(objects.data(), 0, objects.size());
}
