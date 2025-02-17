//
//  RayTracer.m
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "engine/RayTracer.h"
#include "engine/RenderSettings.h"
#include "logger/Logger.h"
#include "threadpool/ThreadPool.h"
#include <stdint.h>

#define kMinHitTime 0.001 // Positive tmin fixes shadow acne.

typedef struct renderPixelArgs_t
{
    uint16_t row;
    uint16_t col;
    Camera *camera;
    Primitive *objects;
    PPMImage *image;
} RenderPixelArgs;


static inline Color3 rayColor(Ray *ray, Primitive *objectsBVH, int depth);
static void renderPixel(void *args);


PPMImage *renderScene(Scene *scene, Camera *camera)
{
    if (!scene || !scene->sceneNode || !camera) return NULL;

    PPMImage *image = makePPMImage(gRenderSettings.pixelsWide, gRenderSettings.pixelsHigh);
    if (!image) return NULL;

    ThreadPool *threadPool = allocThreadPool(gRenderSettings.nthreads);

    RenderPixelArgs args = {.row = 0, .col = 0, .camera = camera, .objects = scene->sceneNode, .image = image};

    LogInfo("Rendering...");

    for (int iRow = 0; iRow < image->height; ++iRow)
    {
        args.row = iRow;

        for (int iCol = 0; iCol < image->width; ++iCol)
        {
            args.col = iCol;
            addTask(threadPool, renderPixel, &args, sizeof(RenderPixelArgs));
        }
    }

    executeTasks(threadPool);
    LogInfo("Render completed.");

    deallocThreadPool(threadPool);
    return image;
}


static inline Color3 rayColor(Ray *ray, Primitive *objectsBVH, int depth)
{
    HitRec hit;

    if (depth <= 0) return color3(0, 0, 0); // Exceeded ray bounce limit.

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
    RenderPixelArgs *pArgs = (RenderPixelArgs *)args;

    const double invSamplesPerPivel = 1.0 / (double)gRenderSettings.samplesPerPixel;

    Color3 pixelColor = color3(0, 0, 0);

    // Sampling:
    for (int iSample = 0; iSample < gRenderSettings.samplesPerPixel; iSample++)
    {
        const double u = (pArgs->col + randomDouble()) / (double)(gRenderSettings.pixelsWide - 1);
        const double v = (pArgs->row + randomDouble()) / (double)(gRenderSettings.pixelsHigh - 1);

        // Generate a new camera ray:
        Ray ray = getRay(pArgs->camera, u, v);

        pixelColor = addVectors(pixelColor, rayColor(&ray, pArgs->objects, gRenderSettings.maxDepth));
    }

    // Set the image's pixel to the average value:
    pArgs->image->pixelValue[pArgs->row][pArgs->col] = scaleVector(pixelColor, invSamplesPerPivel);
}