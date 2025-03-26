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

PhotonEngine::PhotonEngine(unsigned int pixelsWide, unsigned int pixelsHigh) : _writer(pixelsWide, pixelsHigh)
{
}

const PPMWriter *PhotonEngine::render(Scene &scene, Camera &camera)
{
    if (!scene.BVH())
    {
        return nullptr;
    }

    ThreadPool *threadPool = allocThreadPool(computeNumWorkers());

    RenderPixelArgs args = {
        .row = 0, .col = 0, .camera = &camera, .objects = scene.BVH(), .image = const_cast<PPMWriter *>(&_writer)};

    for (int iRow = 0; iRow < _writer.pixelsHigh(); ++iRow)
    {
        args.row = iRow;

        for (int iCol = 0; iCol < _writer.pixelsWide(); ++iCol)
        {
            args.col = iCol;
            addTask(threadPool, renderPixel, &args, sizeof(RenderPixelArgs));
        }
    }

    executeTasks(threadPool);

    deallocThreadPool(threadPool);
    return const_cast<PPMWriter *>(&_writer);
}
