/**
 * @file Hit.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/materials/Material.hpp"
#include "utility/Vector3.hpp"

/* Stores single-hit information */
class Hit
{
public:
    using Time = double;

    /* Returns true if hit lies within range (min, max) */
    bool isValid(Time min, Time max) const;

    static constexpr bool isValid(Time t, Time min, Time max)
    {
        return (min < t && t < max);
    }

    /* Default constructor */
    Hit() = default;

    /* Hit time */
    Time t;

    /* Location of hit */
    Point3 hitPt;

    /* Normal to surface */
    Vector3 normal;

    /* Are we inside or outside object? */
    bool frontFace;

    /* Texture coordinates [0, 1] */
    double u, v;

    /* Surface material */
    Material *material{nullptr};
};