/**
 * @file DielectricMaterial.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Material.hpp"
#include "engine/HitRec.hpp"

/**
 * Dielectric material (i.e. glass, water).
 */
class DielectricMaterial : public Material
{
public:
    DielectricMaterial() = delete;

    DielectricMaterial(double indexOfRefraction);

    /* Refracts an incident ray */
    bool scatter(Ray *incidentRay, Hit *hit, Ray *scatteredRay, Color3 *attenuation) override;

protected:
    double indexOfRefraction;

    /*
     * Schlick's approximation for reflectance. Glass has reflectivity that varies with angle. This is similar to
     * looking at a window at a steep angle---it mostly reflects. This will be when the cosine is small.
     */
    double reflectance(double cosine, double refractionRatio);
};