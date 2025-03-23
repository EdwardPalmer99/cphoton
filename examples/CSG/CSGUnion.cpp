/**
 * @file Spheres.C
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

extern "C"
{
#include "engine/RenderSettings.h"
}

#include "engine/PhotonEngine.hpp"
#include "engine/Scene.hpp"
#include "engine/materials/MatteMaterial.hpp"
#include "engine/materials/MetalMaterial.hpp"
#include "engine/primitives/CSGNode.hpp"
#include "engine/primitives/Plane.hpp"
#include "engine/primitives/Primitive.hpp"
#include "engine/primitives/Sphere.hpp"

int main(int argc, const char *argv[])
{
    gRenderSettings.pixelsWide = 500;
    gRenderSettings.pixelsHigh = 500;

    parseCLIOptions(argc, argv);

    // Create the camera:
    const double aspectRatio = ((double)gRenderSettings.pixelsWide / (double)gRenderSettings.pixelsHigh);
    Camera camera(45.0, aspectRatio, 1, 0, point3(0, 4, 4), point3(0, 1, 0));

    Primitive *sphere1 = new Sphere(point3(+0.25, 1, 0), 1, std::make_shared<MetalMaterial>(color3(0, 1, 0)));
    Primitive *sphere2 = new Sphere(point3(-0.25, 1, 0), 1, std::make_shared<MetalMaterial>(color3(1, 0, 0)));

    Primitive *plane =
        new Plane(point3(0, 0, 0), point3(0, 1, 0), std::make_shared<MatteMaterial>(color3(0.1, 0.1, 0.1)));

    Primitive *CSG = new CSGNode(sphere1, sphere2, CSGNode::CSGUnion);

    Scene scene;
    scene.addObject(CSG);
    scene.addObject(plane);

    PhotonEngine engine(gRenderSettings.pixelsWide, gRenderSettings.pixelsHigh);

    PPMImage *outputImage = engine.render(&scene, &camera);
    writeBinary16BitPPMImage(outputImage, gRenderSettings.outputPath);
    freePPMImage(outputImage);

    return 0;
}