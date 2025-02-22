/**
 * @file Spheres.C
 * @author Edward Palmer
 * @date 2025-02-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "CPhoton.h"

 int main(int argc, const char *argv[])
{
    gRenderSettings.pixelsWide = 250;
    gRenderSettings.pixelsHigh = 250;

    parseCLIOptions(argc, argv);

    // Create the camera:
    const double aspectRatio = ((double)gRenderSettings.pixelsWide / (double)gRenderSettings.pixelsHigh);
    Camera camera = makeCamera(45.0, aspectRatio, 1, 0, point3(0, 4, 4), point3(0, 1, 0));

    Primitive *sphere1 = makeSphere(point3(1, 1, 0), 1, makeMetal(makeSolidTexture(color3(0, 1, 0)), 0));
    Primitive *sphere2 = makeSphere(point3(-1, 1, 0), 1, makeMetal(makeSolidTexture(color3(1, 0, 0)), 0));
    Primitive *plane = makePlane(point3(0, 0, 0), point3(0, 1, 0), makeLambertianWithColor(color3(0.1, 0.1, 0.1)));

    Scene *scene = makeScene();

    scene->addObject(scene, sphere1);
    scene->addObject(scene, sphere2);
    scene->addObject(scene, plane);

    scene->markAsFinished(scene);

    PPMImage *outputImage = renderScene(scene, &camera);

    writeBinary16BitPPMImage(outputImage, gRenderSettings.outputPath);

    scene->destructor(scene);
    freePPMImage(outputImage);

    return 0;
}