/**
 * @file MatteMaterial.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Material.hpp"
#include "engine/Hit.hpp"
#include "engine/textures/Texture.hpp"
#include "utility/Vector3.h"
#include <memory>

/**
 * Material class for a matte material ("lambertian").
 */
class MatteMaterial : public Material
{
public:
    MatteMaterial() = delete;
    MatteMaterial(std::shared_ptr<Texture> albedo);
    MatteMaterial(Color3 color);

    /* Scatters ray */
    bool scatter(Ray *incidentRay, Hit *hit, Ray *scatteredRay, Color3 *attenuation) override;

protected:
    std::shared_ptr<Texture> albedo{nullptr};
};