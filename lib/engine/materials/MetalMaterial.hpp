/**
 * @file MetalMaterial.hpp
 * @author Edward Palmer
 * @date 2025-03-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Material.hpp"
#include "engine/Hit.hpp"
#include "engine/textures/Texture.hpp"
#include <memory>

// TODO: - instead of shared pointers for textures, we can store a reference.

/**
 * Class for a metal material (reflective).
 */
class MetalMaterial : public Material
{
public:
    MetalMaterial() = delete;
    MetalMaterial(std::shared_ptr<Texture> albedo, double fuzziness = 0.0);
    MetalMaterial(Color3 color, double fuzziness = 0.0);

    /* Reflects incoming ray */
    bool scatter(Ray &incidentRay, Hit &hit, Ray &scatteredRay, Color3 &attenuation) override;

protected:
    std::shared_ptr<Texture> albedo{nullptr};
    double fuzziness;
};