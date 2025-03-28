/**
 * @file PhotonEngine.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "engine/PhotonEngine.hpp"
#include "engine/PhotonEngineImpl.hpp"
#include "engine/primitives/BVHNode.hpp"

#include <stdint.h>

extern "C"
{
#include "threadpool/ThreadPool.h"
#include "threadpool/ThreadUtils.h"
}

PhotonEngine::PhotonEngine(unsigned int pixelsWide_, unsigned int pixelsHigh_)
    : pixelsWide(pixelsWide_), pixelsHigh(pixelsHigh_)
{
}

PPMImage *PhotonEngine::render(Scene &scene, Camera &camera) const
{
    if (!scene.BVH())
    {
        return nullptr;
    }

    PPMImage *image = makePPMImage(pixelsWide, pixelsHigh);
    if (!image) return nullptr;

    ThreadPool *threadPool = allocThreadPool(computeNumWorkers());

    RenderPixelArgs args = {.row = 0, .col = 0, .camera = &camera, .objects = scene.BVH(), .image = image};

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
