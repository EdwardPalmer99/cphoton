/**
 * @file PhotonEngine.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "engine/Camera.hpp"
#include "engine/Scene.hpp"

extern "C"
{
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
    PPMImage *render(Scene &scene, Camera &camera) const;

private:
    unsigned int pixelsWide;
    unsigned int pixelsHigh;
};
