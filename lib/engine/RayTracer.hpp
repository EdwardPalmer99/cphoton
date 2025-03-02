/**
 * @file RayTracer.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

extern "C"
{
#include "engine/Camera.h"
#include "engine/Scene.h"
#include "utility/PPMWriter.h"
}

class PhotonEngine
{
public:
    PhotonEngine() = delete;
    PhotonEngine(unsigned int pixelsWide_, unsigned int pixelsHigh_);

    /**
     * @brief Renders a scene.
     */
    PPMImage *render(Scene *scene, Camera *camera) const;

private:
    unsigned int pixelsWide;
    unsigned int pixelsHigh;
};
