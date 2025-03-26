/**
 * @file MatteMaterial.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "MatteMaterial.hpp"
#include "engine/textures/SolidTexture.hpp"

MatteMaterial::MatteMaterial(std::shared_ptr<Texture> albedo_) : albedo(albedo_)
{
}


MatteMaterial::MatteMaterial(Color3 color_) : MatteMaterial(std::make_shared<SolidTexture>(color_))
{
}


bool MatteMaterial::scatter(Ray &incidentRay, Hit &hit, Ray &scatteredRay, Color3 &attenuation)
{
    // Compute random scattering direction:
    Vector3 scatterDirection = hit.normal + Vector3::randomUnitVector();

    // If random vector is in opposite direction to normal they will sum to
    // a zero vector. Set to normal direction if this happens.
    if (scatterDirection.isApproxZero())
    {
        scatterDirection = hit.normal;
    }

    scatteredRay = Ray(hit.hitPt, scatterDirection);
    attenuation = albedo->value(hit.u, hit.v, &hit.hitPt);

    return true;
}
