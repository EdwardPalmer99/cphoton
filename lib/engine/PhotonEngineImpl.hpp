/**
 * @file PhotonEngineImpl.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/Camera.hpp"
#include "engine/Ray.hpp"
#include "engine/primitives/Primitive.hpp"
#include "utility/PPMWriter.hpp"
#include "utility/Vector3.hpp"

#include <stdint.h>

/** Struct passed to renderPixel function */
struct RenderPixelArgs
{
    uint16_t row;
    uint16_t col;
    Camera *camera;
    Primitive *objects;
    PPMWriter *image;
};

/**
 * @brief Renders a single pixel by repeatedly firing rays for a pixel and sampling the colors. Function is called by a
 * worker in a thread pool.
 * @param args is a pointer to the RenderPixelArgs struct cast to (void *)
 */
void renderPixel(void *args);

/**
 * @brief Computes the ray color for a single pixel and sample.
 * @param ray is the ray being fired
 * @param objectsBVH is the object containing all primitives in the scene
 * @param depth is the maximum number of "bounces"
 * @return Color3 is the color of the returned ray
 */
Color3 rayColor(Ray &ray, Primitive *objectsBVH, int depth);
