/**
 * @file SolidTexture.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "SolidTexture.hpp"
#include <utility>


SolidTexture::SolidTexture(Color3 color_) : color(std::move(color_))
{
}

SolidTexture::SolidTexture(PresetColor colorType)
{
    switch (colorType)
    {
        case Gold:
            color = Color3(1.000, 0.766, 0.336);
            break;
        case Silver:
            color = Color3(0.972, 0.960, 0.816);
            break;
        case Grey:
        default:
            color = Color3(0.20, 0.26, 0.35);
            break;
    }
}


Color3 SolidTexture::value(double u, double v, Point3 *hitPt)
{
    return color;
}
