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
#include "engine/materials/Materials.hpp"
#include "engine/primitives/Primitives.hpp"
#include "engine/textures/Textures.hpp"
#include <string>

int main(int argc, const char *argv[])
{
    RenderSettings::instance().setDefaultWidthHeight(500, 500);
    parseCLIOptions(argc, argv);

    // Create the camera:
    Camera camera(45.0, RenderSettings::instance().aspectRatio(), 1, 0, point3(-2, 3, 4), point3(0, 1, 0));

    Primitive *cube1 =
        new Cube(point3(0.5, 1, 0), vector3(22.5, 0, 0), 1, std::make_shared<MetalMaterial>(color3(0, 1, 0)));
    Primitive *cube2 =
        new Cube(point3(0, 1, 0), vector3(-22.5, 0, 0), 1, std::make_shared<MetalMaterial>(color3(1, 0, 0)));

    Primitive *plane =
        new Plane(point3(0, 0, 0), point3(0, 1, 0), std::make_shared<MatteMaterial>(color3(0.1, 0.1, 0.1)));

    Primitive *CSG = new CSGNode(cube1, cube2, CSGNode::CSGUnion);

    Scene scene;
    scene.addObject(CSG);
    scene.addObject(plane);

    PhotonEngine engine(RenderSettings::instance().pixelsWide, RenderSettings::instance().pixelsHigh);

    const PPMWriter *writer = engine.render(scene, camera);
    if (writer)
    {
        writer->write(std::string(RenderSettings::instance().outputPath));
    }

    return 0;
}