/**
 * @file Spheres.C
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "engine/CLIOptions.hpp"
#include "engine/PhotonEngine.hpp"
#include "engine/RenderSettings.hpp"
#include "engine/Scene.hpp"
#include "engine/primitives/CSGNode.hpp"
#include "engine/primitives/Plane.hpp"
#include "engine/primitives/Primitive.hpp"
#include "engine/primitives/Sphere.hpp"

#include "engine/materials/MatteMaterial.hpp"
#include "engine/materials/MetalMaterial.hpp"
#include "engine/textures/SolidTexture.hpp"

int main(int argc, const char *argv[])
{
    RenderSettings::instance().setDefaultWidthHeight(500, 500);
    parseCLIOptions(argc, argv);

    // Create the camera:
    Camera camera(45.0, RenderSettings::instance().aspectRatio(), 1, 0, point3(0, 4, 4), point3(0, 1, 0));

    Primitive *sphere1 = new Sphere(point3(0, 1, 0), 1,
                                    std::make_shared<MetalMaterial>(std::make_shared<SolidTexture>(color3(0, 1, 0))));
    Primitive *sphere2 = new Sphere(point3(0, 1, 0.5), 1,
                                    std::make_shared<MetalMaterial>(std::make_shared<SolidTexture>(color3(0, 1, 0))));

    Primitive *plane =
        new Plane(point3(0, 0, 0), point3(0, 1, 0),
                  std::make_shared<MatteMaterial>(std::make_shared<SolidTexture>(color3(0.1, 0.1, 0.1))));

    Primitive *CSG = new CSGNode(sphere1, sphere2, CSGNode::CSGDifference);

    Scene scene;
    scene.addObject(CSG);
    scene.addObject(plane);

    PhotonEngine engine(RenderSettings::instance().pixelsWide, RenderSettings::instance().pixelsHigh);

    PPMImage *outputImage = engine.render(&scene, &camera);
    writeBinary16BitPPMImage(outputImage, RenderSettings::instance().outputPath);
    freePPMImage(outputImage);

    return 0;
}