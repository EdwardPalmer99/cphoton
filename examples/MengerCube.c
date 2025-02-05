/**
 * @file MengerCube.c
 * @author Edward Palmer
 * @date 2025-01-31
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "models/MengerCube.h"
#include "engine/Camera.h"
#include "engine/Material.h"
#include "engine/RayTracer.h"
#include "engine/RenderSettings.h"
#include "engine/Scene.h"
#include "engine/Texture.h"
#include "utility/PPMWriter.h"

int main(int argc, const char *argv[])
{
    gRenderSettings.pixelsWide = 800;
    gRenderSettings.pixelsHigh = 600;
    parseCLIOptions(argc, argv);

    // Create the camera:
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
