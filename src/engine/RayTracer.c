//
//  RayTracer.m
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "RayTracer.h"
#include "ThreadPool.h"

#define kMinHitTime 0.001 // Positive tmin fixes shadow acne.

static inline Color3 rayColor(Ray *ray, Primitive *objectsBVH, int depth);

typedef struct
{
    PPMImage *image;
    size_t iRow;
    size_t iCol;
    size_t samplesPerPixel;
    int maxDepth;
    Camera *camera;
    Primitive *sceneNode;
} ImageInfo;


void renderRow(void *args)
{
    ImageInfo *info = (ImageInfo *)args;

    size_t iRow = info->iRow;
    size_t iCol = info->iCol;

    // fprintf(stdout, "rendering row %zu\n", iRow);

    const double invSamplesPerPivel = 1.0 / (double)info->samplesPerPixel;

    Color3 pixelColor = color3(0, 0, 0);

    // Sampling:
    for (int iSample = 0; iSample < info->samplesPerPixel; iSample++)
    {
        const double u = (iCol + randomDouble()) / (double)(info->image->width - 1);
        const double v = (iRow + randomDouble()) / (double)(info->image->height - 1);

        // Generate a new camera ray:
        Ray ray = getRay(info->camera, u, v);

        pixelColor = addVectors(pixelColor, rayColor(&ray, info->sceneNode, info->maxDepth));
    }

    // Set the image's pixel to the average value:
    info->image->pixelValue[iRow][iCol] = scaleVector(pixelColor, invSamplesPerPivel);
}

PPMImage *renderScene(Scene *scene, Camera *camera, int imageWidth, int imageHeight,
                      int samplesPerPixel, int maxDepth)
{
    if (!scene || !scene->sceneNode || !camera)
        return NULL;

    Primitive *sceneNode = scene->sceneNode; // Contains all objects.

    PPMImage *image = makePPMImage(imageWidth, imageHeight);
    if (!image) return NULL;

    ThreadPool *threadPool = allocThreadPool(8);

    ImageInfo args = {.image = image, .iRow = 0, .iCol = 0, .samplesPerPixel = samplesPerPixel, .maxDepth = maxDepth, .camera = camera, .sceneNode = sceneNode};

    for (int iRow = 0; iRow < image->height; ++iRow)
    {
        args.iRow = iRow;

        for (int iCol = 0; iCol < image->width; ++iCol)
        {
            args.iCol = iCol;
            addTask(threadPool, renderRow, &args, sizeof(ImageInfo));
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
