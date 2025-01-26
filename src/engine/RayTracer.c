//
//  RayTracer.m
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "RayTracer.h"
#include "PixelLocation.h"
#include "ThreadPool.h"
#include <stdint.h>

#define kMinHitTime 0.001 // Positive tmin fixes shadow acne.

typedef struct globalRenderSettings_t
{
    Camera *camera;
    Primitive *sceneNode;
    PPMImage *image;
    size_t samplesPerPixel;
    size_t maxDepth;
} GlobalRenderSettings;

static GlobalRenderSettings global;


static inline Color3 rayColor(Ray *ray, Primitive *objectsBVH, int depth);
static void renderPixel(void *args);


PPMImage *renderScene(Scene *scene, Camera *camera, int imageWidth, int imageHeight,
                      int samplesPerPixel, int maxDepth)
{
    if (!scene || !scene->sceneNode || !camera)
        return NULL;

    Primitive *sceneNode = scene->sceneNode; // Contains all objects.

    PPMImage *image = makePPMImage(imageWidth, imageHeight);
    if (!image) return NULL;

    // Setup global variable:
    {
        global.camera = camera;
        global.sceneNode = sceneNode;
        global.image = image;
        global.samplesPerPixel = samplesPerPixel;
        global.maxDepth = maxDepth;
    }

    ThreadPool *threadPool = allocThreadPool(8);

    PixelLocation args = {.row = 0, .col = 0};

    for (int iRow = 0; iRow < image->height; ++iRow)
    {
        args.row = iRow;

        for (int iCol = 0; iCol < image->width; ++iCol)
        {
            args.col = iCol;
            addTask(threadPool, renderPixel, &args, sizeof(PixelLocation));
        }
    }

    executeTasks(threadPool);
    deallocThreadPool(threadPool);

    return image;
}


static inline Color3 rayColor(Ray *ray, Primitive *objectsBVH, int depth)
{
    HitRec hit;

    if (depth <= 0)
        return color3(0, 0, 0); // Exceeded ray bounce limit.

    if (objectsBVH->hit(objectsBVH, ray, kMinHitTime, INFINITY, &hit))
    {
        Ray scatteredRay;
        Color3 attenuation;
        Color3 emitted = hit.material->emitted;

        if (hit.material->scatter(ray, &hit, &scatteredRay, &attenuation))
        {
            Color3 outputColor = rayColor(&scatteredRay, objectsBVH, depth - 1);

            return addVectors(emitted, multiplyColors(outputColor, attenuation));
        }
        else
        {
            return emitted;
        }
    }
    else
    {
        // Didn't hit anything. Return the background color for the sky:
        const double t = 0.5 * (unitVector(ray->direction).y + 1.0);

        Color3 whiteComponent = scaleVector(color3(1, 1, 1), 1 - t);
        Color3 blueComponent = scaleVector(color3(0.5, 0.7, 1.0), t);

        return addVectors(whiteComponent, blueComponent);
    }
}


static void renderPixel(void *args)
{
    PixelLocation *loc = (PixelLocation *)args;

    const double invSamplesPerPivel = 1.0 / (double)global.samplesPerPixel;

    Color3 pixelColor = color3(0, 0, 0);

    // Sampling:
    for (int iSample = 0; iSample < global.samplesPerPixel; iSample++)
    {
        const double u = (loc->col + randomDouble()) / (double)(global.image->width - 1);
        const double v = (loc->row + randomDouble()) / (double)(global.image->height - 1);

        // Generate a new camera ray:
        Ray ray = getRay(global.camera, u, v);

        pixelColor = addVectors(pixelColor, rayColor(&ray, global.sceneNode, global.maxDepth));
    }

    // Set the image's pixel to the average value:
    global.image->pixelValue[loc->row][loc->col] = scaleVector(pixelColor, invSamplesPerPivel);
}