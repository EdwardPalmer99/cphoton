/**
 * @file EmitterMaterial.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "EmitterMaterial.hpp"
#include <utility>


EmitterMaterial::EmitterMaterial(Color3 color_) : color(std::move(color_))
{
}


bool EmitterMaterial::scatter(Ray &incidentRay, Hit &hit, Ray &scatteredRay, Color3 &attenuation)
{
    // Absorb ray:
    return false;
}


Color3 EmitterMaterial::emitted() const
{
    return color;
}