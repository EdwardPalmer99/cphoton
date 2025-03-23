/**
 * @file Material.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Material.hpp"

Color3 Material::emitted() const
{
    return color3(0, 0, 0);
}


Vector3 Material::reflect(Vector3 v, Vector3 n)
{
    // Reflected vector: v - 2*(v.n)*n
    return subtractVectors(v, scaleVector(n, 2.0 * dot(v, n)));
}


Vector3 Material::refract(Vector3 vin, Vector3 n, double refractionRatio)
{
    Vector3 vinParallel = scaleVector(n, dot(vin, n)); // Parallel to normal component, n.
    Vector3 vinPerpendicular = subtractVectors(vin, vinParallel);

    Vector3 voutPerpendicular = scaleVector(vinPerpendicular, refractionRatio);
    Vector3 voutParallel = scaleVector(n, -sqrt(1.0 - lengthSquared(voutPerpendicular)));

    return addVectors(voutPerpendicular, voutParallel);
}