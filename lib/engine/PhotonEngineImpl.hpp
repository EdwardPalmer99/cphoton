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

extern "C"
{
#include "engine/Primitive.h"
#include "engine/Ray.h"
#include "utility/PPMWriter.h"
#include "utility/Vector3.h"
}

#include <stdint.h>

/** Struct passed to renderPixel function */
typedef struct
{
    uint16_t row;
    uint16_t col;
    Camera *camera;
    Primitive *objects;
    PPMImage *image;
} RenderPixelArgs;

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
Color3 rayColor(Ray *ray, Primitive *objectsBVH, int depth);
