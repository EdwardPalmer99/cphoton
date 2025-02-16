#include "SceneGenerator.hpp"
#include "YAMLParser.hpp"

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

    // Set renderer settings.
    sceneGenerator.configRenderer();

    // Setup camera.
    Camera camera = sceneGenerator.buildCamera();

    // Create scene.
    Scene *scene = sceneGenerator.buildScene();

    // Do render.
    PPMImage *outputImage = renderScene(scene, &camera);

    // Write to output image.
    writeBinary16BitPPMImage(outputImage, gRenderSettings.outputPath);

    // Cleanup.
    scene->destructor(scene);
    freePPMImage(outputImage);

    return 0;
}