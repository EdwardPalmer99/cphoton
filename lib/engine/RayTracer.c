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
#include "threadpool/ThreadUtils.h"
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

    ThreadPool *threadPool = allocThreadPool(computeNumWorkers());

    RenderPixelArgs args = {.row = 0, .col = 0, .camera = camera, .objects = scene->sceneNode, .image = image};

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
    /**
     * References:
     * - https://cs184.eecs.berkeley.edu/sp24/docs/hw3-1-part-5
     * - https://en.wikipedia.org/wiki/Z-test
     *
     * Z-test @ 95% confidence interval --> Z = 1.96.
     *
     * Z = 1.96 = |X_bar - mu_0| / s where s^2 = sigma^2/N, X_bar is sample mean, mu_0 is population mean
     *
     * Let delta = |X_bar - mu_0| (the absolute value between our sample mean and population mean @ 95% confidence
     *
     * We would like delta < 5% * mu_0
     *
     * Define s1 = sum(X)
     * Define s2 = sum(X^2)
     *
     * Approximate mu_0 ≈ X_bar for large N, X_bar=s1/N
     *
     * --> delta = 1.96*s = 1.96*sigma/sqrt(N) < 5%*(s1/N)
     *
     * sigma^2 = 1/N * (s2 - s1^2/N)
     */

    static const int kMaxDepth = 50;
    static const int kMinSample = 30; // Need sufficient number to approximate Normal distribution.
    static const int kMaxSample = 10000;
    static const int kSampleBatch = 10;

    RenderPixelArgs *pArgs = (RenderPixelArgs *)args;

    Color3 pixelColor = color3(0, 0, 0);

    double s1 = 0.0; // Sum of values.
    double s2 = 0.0; // Sum of squares of values.

    int numSamples = 1;

    // Sampling:
    for (; numSamples <= kMaxSample; ++numSamples)
    {
        const double u = (pArgs->col + randomDouble()) / (double)(gRenderSettings.pixelsWide - 1);
        const double v = (pArgs->row + randomDouble()) / (double)(gRenderSettings.pixelsHigh - 1);

        // Generate a new camera ray:
        Ray ray = getRay(pArgs->camera, u, v);

        Color3 color = rayColor(&ray, pArgs->objects, kMaxDepth);

        // Compute the luminance:
        // https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color
        {
            double luminance = 0.21 * color.r + 0.72 * color.g + 0.07 * color.b;

            s1 += luminance;
            s2 += (luminance * luminance);
        }

        pixelColor = addVectors(pixelColor, color);

        // Recalculate the metric periodically to see if we need additional samples.
        // NB: ensure we have sufficient samples first to have a good figure.
        if (numSamples >= kMinSample && (numSamples % kSampleBatch == 0))
        {
            double invNumSamples = 1.0 / (double)numSamples;

            double sigmaSquared = invNumSamples * (s2 - (s1 * s1) * invNumSamples);
            double deltaSquared = (1.96 * 1.96) * sigmaSquared * invNumSamples;

            double thresholdSquared = pow(0.05 * s1 * invNumSamples, 2.0);

            if (deltaSquared < thresholdSquared)
            {
                break;
            }
        }
    }

    // Set the image's pixel to the average value:
    pArgs->image->pixelValue[pArgs->row][pArgs->col] = scaleVector(pixelColor, 1.0 / (double)numSamples);
}