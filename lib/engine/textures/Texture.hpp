/**
 * @file Texture.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "utility/Vector3.hpp"

/* Base texture class. */
class Texture
{
public:
    /* Returns the color at a given point */
    virtual Color3 value(double u, double v, Point3 *hitPt) = 0;

protected:
    /* NB: protect to avoid default initialization. */
    Texture() = default;
};