/**
 * @file EmitterMaterial.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Material.hpp"
#include "engine/Hit.hpp"

/* Emitter (i.e. light source)*/
class EmitterMaterial : public Material
{
public:
    EmitterMaterial() = delete;
    EmitterMaterial(Color3 color);

    /* Absorb incident ray (return false since ray totally absorbed) */
    bool scatter(Ray *incidentRay, Hit *hit, Ray *scatteredRay, Color3 *attenuation) override;

    /* Returns light-source color */
    Color3 emitted() const override;

protected:
    /* Light-source color */
    Color3 color;
};