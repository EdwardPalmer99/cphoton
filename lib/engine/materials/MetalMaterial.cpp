/**
 * @file MetalMaterial.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "MetalMaterial.hpp"
#include "engine/textures/SolidTexture.hpp"

extern "C"
{
#include "utility/MathMacros.h"
}

MetalMaterial::MetalMaterial(std::shared_ptr<Texture> albedo_, double fuzziness_)
    : albedo(albedo_), fuzziness(fuzziness_)
{
    fuzziness = clamp(fuzziness_, 0, 1);
}


MetalMaterial::MetalMaterial(Color3 color_, double fuzziness_)
    : MetalMaterial(std::make_shared<SolidTexture>(color_), fuzziness_)
{
}


bool MetalMaterial::scatter(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation)
{
    Vector3 reflectDirection = reflect(unitVector(incidentRay->direction), hit->normal);

    // Fuzzy reflections are created by choosing a new endpoint in a unit sphere.
    if (fuzziness > 0.0)
    {
        Vector3 changeToVector = scaleVector(randomUnitSphereVector(), fuzziness);
        reflectDirection = addVectors(reflectDirection, changeToVector);
    }

    *scatteredRay = Ray(hit->hitPt, reflectDirection);
    *attenuation = albedo->value(hit->u, hit->v, &hit->hitPt);

    // Make sure that the scattered ray is not scattering into the object:
    return (dot(reflectDirection, hit->normal) > 0.0);
}
