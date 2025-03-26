/**
 * @file Material.hpp
 * @author Edward Palmer
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/Ray.hpp"
#include "utility/Vector3.hpp"

// Forward declaration:
struct Hit;

/**
 * Base material class.
 */
class Material
{
public:
    /* Sets the scatteredRay and attenuation */
    virtual bool scatter(Ray &incidentRay, Hit &hit, Ray &scatteredRay, Color3 &attenuation) = 0;

    /* Returns black color (material doesn't emit light). */
    virtual Color3 emitted() const;

protected:
    /* Protect default constructor to avoid direct initialization */
    Material() = default;

    Vector3 reflect(const Vector3 &v, const Vector3 &n) const;

    Vector3 refract(const Vector3 &vin, const Vector3 &n, double refractionRatio) const;
};