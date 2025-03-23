/**
 * @file HitRec.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/materials/Material.hpp"

extern "C"
{
#include "utility/Vector3.h"
}

struct HitRec
{
    double t;     // Time of hit.
    Point3 hitPt; // Location of hit.
    Vector3 normal;
    bool frontFace;
    double u, v; // Texture coordinates [0, 1].
    Material *material{nullptr};
};