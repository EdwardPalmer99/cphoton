/**
 * @file CheckerTexture.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "CheckerTexture.hpp"

CheckerTexture::CheckerTexture(std::shared_ptr<Texture> odd_, std::shared_ptr<Texture> even_) : odd(odd_), even(even_)
{
}


Color3 CheckerTexture::value(double u, double v, Point3 *hitPt)
{
    const double sines = sin(10.0 * hitPt->x) * sin(10.0 * hitPt->y) * sin(10.0 * hitPt->z);

    if (sines < 0.0)
        return odd->value(u, v, hitPt);
    else
        return even->value(u, v, hitPt);
}
