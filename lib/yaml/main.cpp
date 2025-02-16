#include "SceneGenerator.hpp"
#include "YAMLParser.hpp"
#include <iostream>

extern "C"
{
#include "engine/Primitive.h"
#include "engine/RayTracer.h"
#include "engine/RenderSettings.h"
#include "utility/PPMWriter.h"
}

int main(void)
{
    SceneGenerator sceneGenerator("TestFile.yaml");
    std::cout << "done parsing" << std::endl;

    // Set renderer settings.
    sceneGenerator.configRenderer();
    std::cout << "done config" << std::endl;

    // Setup camera.
    Camera camera = sceneGenerator.buildCamera();
    std::cout << "done camera" << std::endl;

    // Create scene.
    Scene *scene = sceneGenerator.buildScene();
    std::cout << "done scene" << std::endl;

    // Do render.
    PPMImage *outputImage = renderScene(scene, &camera);

    // Write to output image.
    writeBinary16BitPPMImage(outputImage, gRenderSettings.outputPath);

    // Cleanup.
    scene->destructor(scene);
    freePPMImage(outputImage);

    return 0;
}