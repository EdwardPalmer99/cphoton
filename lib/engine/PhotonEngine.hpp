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
#include "utility/PPMWriter.hpp"

class PhotonEngine
{
public:
    PhotonEngine() = delete;
    PhotonEngine(unsigned int pixelsWide, unsigned int pixelsHigh);

    /**
     * @brief Renders a scene.
     */
    const PPMWriter *render(Scene &scene, Camera &camera);

private:
    PPMWriter _writer;
};
