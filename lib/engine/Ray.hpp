/**
 * @file Ray.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

extern "C"
{
#include "utility/Vector3.h"
}

struct Ray
{
    /* Constructor */
    Ray() = default;
    Ray(Point3 origin, Vector3 direction);

    /* Returns the ray at a time t */
    Point3 pointAtTime(double t);

    Point3 origin;
    Vector3 direction;
};