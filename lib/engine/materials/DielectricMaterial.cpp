/**
 * @file DielectricMaterial.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "DielectricMaterial.hpp"

extern "C"
{
#include "utility/Randomizer.h"
}


DielectricMaterial::DielectricMaterial(double indexOfRefraction_) : indexOfRefraction(indexOfRefraction_)
{
}


bool DielectricMaterial::scatter(Ray *incidentRay, HitRec *hit, Ray *scatteredRay, Color3 *attenuation)
{
    Vector3 unitDirection = unitVector(incidentRay->direction);

    // Angle theta is the angle between the normal and incident ray.
    const double cosTheta = dot(flipVector(unitDirection), hit->normal);
    const double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // If it's the front face, we're going into the object otherwise we're leaving
    // and exiting into the air.
    const double ir = indexOfRefraction;
    const double refractionRatio =
        hit->frontFace ? (1.0 / ir) : ir; // TODO: - we can calculate based on incident ray and normal whether this was
                                          // inside or outside object. Don't need this.

    // Total internal reflection:
    bool cannotRefract = (refractionRatio * sinTheta > 1.0);

    Vector3 direction;

    if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble())
        direction = reflect(unitDirection, hit->normal);
    else
        direction = refract(unitDirection, hit->normal, refractionRatio);

    *scatteredRay = makeRay(hit->hitPt, direction);
    *attenuation = color3(1.0, 1.0, 1.0);

    return true;
}


double DielectricMaterial::reflectance(double cosine, double refractionRatio)
{
    double r0 = (1.0 - refractionRatio) / (1.0 + refractionRatio);

    r0 = r0 * r0;

    return r0 + (1.0 - r0) * pow(1.0 - cosine, 5);
}