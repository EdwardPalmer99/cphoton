/**
 * @file SolidTexture.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Texture.hpp"
#include "utility/Vector3.hpp"

class SolidTexture : public Texture
{
public:
    SolidTexture() = delete;
    SolidTexture(Color3 color);

    enum PresetColor
    {
        Gold,
        Silver,
        Grey
    };

    SolidTexture(PresetColor colorType);

    // TODO: - would be better if we could return a reference.
    Color3 value(double u, double v, Point3 *hitPt) override;

protected:
    Color3 color;
};