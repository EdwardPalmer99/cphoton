/**
 * @file MengerCube.c
 * @author Edward Palmer
 * @date 2025-01-31
 *
 * @copyright Copyright (c) 2025
 *
 */

extern "C"
{
#include "utility/PPMWriter.h"
}

#include "engine/CLIOptions.hpp"
#include "engine/PhotonEngine.hpp"
#include "engine/RenderSettings.hpp"
#include "engine/Scene.hpp"
#include "engine/materials/MatteMaterial.hpp"
#include "engine/materials/MetalMaterial.hpp"
#include "engine/primitives/BVHNode.hpp"
#include "engine/primitives/Cube.hpp"
#include "engine/primitives/Plane.hpp"
#include "engine/primitives/Primitive.hpp"
#include "engine/textures/SolidTexture.hpp"
#include "models/MengerCube.hpp"

int main(int argc, const char *argv[])
{
    RenderSettings::instance().setDefaultWidthHeight(800, 600);
    parseCLIOptions(argc, argv);

    // Create the camera:
    Camera camera(45.0, RenderSettings::instance().aspectRatio(), 4, 0.0, point3(2, 5, 5), point3(0.2, 0.6, 1.0));

    // Create the textures:
    auto greyColor = std::make_shared<SolidTexture>(color3(0.20, 0.26, 0.35));
    auto goldColor = std::make_shared<SolidTexture>(SolidTexture::Gold);

    // Create the materials:
    auto greyMetal = std::make_shared<MetalMaterial>(greyColor, 0.2);
    auto goldLambertian = std::make_shared<MatteMaterial>(goldColor);

    // Create the scene:
    Scene scene;

    Primitive *mengerSponge0 = makeMengerSponge(0, point3(-1.5, 0.5, -1.5), 1.0, goldLambertian);
    Primitive *mengerSponge1 = makeMengerSponge(1, point3(1.5, 0.5, -1.5), 1.0, goldLambertian);
    Primitive *mengerSponge3 = makeMengerSponge(3, point3(-1.5, 0.5, 1.5), 1.0, goldLambertian);
    Primitive *mengerSponge2 = makeMengerSponge(2, point3(1.5, 0.5, 1.5), 1.0, goldLambertian);
    Primitive *mengerSponge4 = makeMengerSponge(4, point3(0, 0.5, 0), 1.0, goldLambertian);
    Primitive *plane = new Plane(point3(0, 0, 0), vector3(0, 1, 0), greyMetal);

    scene.addObject(mengerSponge0);
    scene.addObject(mengerSponge1);
    scene.addObject(mengerSponge2);
    scene.addObject(mengerSponge3);
    scene.addObject(mengerSponge4);
    scene.addObject(plane);

    // Render:
    PhotonEngine engine(RenderSettings::instance().pixelsWide, RenderSettings::instance().pixelsHigh);

    PPMImage *outputImage = engine.render(&scene, &camera);
    writeBinary16BitPPMImage(outputImage, RenderSettings::instance().outputPath);

    // Cleanup:
    freePPMImage(outputImage);
    return 0;
}
