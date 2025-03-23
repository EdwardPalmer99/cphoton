/**
 * @file Material.hpp
 * @author Edward Palmer
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

// Forward declaration:
struct HitRec;

extern "C"
{
#include "engine/Ray.h"
#include "utility/Vector3.h"
}

/**
 * Base material class.
 */
class Material
{
public:
    /* Sets the scatteredRay and attenuation */
    virtual bool scatter(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation) = 0;

    /* Returns black color (material doesn't emit light). */
    virtual Color3 emitted() const;

protected:
    /* Protect default constructor to avoid direct initialization */
    Material() = default;

    Vector3 reflect(Vector3 v, Vector3 n);

    Vector3 refract(Vector3 vin, Vector3 n, double refractionRatio);
};