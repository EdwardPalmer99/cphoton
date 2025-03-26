/**
 * @file Material.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Material.hpp"
#include <cmath>

Color3 Material::emitted() const
{
    return Color3(0, 0, 0);
}


Vector3 Material::reflect(const Vector3 &v, const Vector3 &n) const
{
    // Reflected vector: v - 2*(v.n)*n
    return v - n * (2.0 * v.dot(n));
}


Vector3 Material::refract(const Vector3 &vin, const Vector3 &n, double refractionRatio) const
{
    Vector3 vinParallel = n * vin.dot(n); // Parallel to normal component, n.
    Vector3 vinPerpendicular = vin - vinParallel;

    Vector3 voutPerpendicular = vinPerpendicular * refractionRatio;
    Vector3 voutParallel = n * (-std::sqrt(1.0 - voutPerpendicular.lengthSquared()));

    return (voutPerpendicular + voutParallel);
}