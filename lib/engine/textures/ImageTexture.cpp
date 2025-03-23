/**
 * @file ImageTexture.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ImageTexture.hpp"

extern "C"
{
#include "utility/MathMacros.h"
}


ImageTexture::ImageTexture(uint8_t *bytes_, size_t pixelsWide_, size_t pixelsHigh_, size_t bitsPerPixel_)
    : bytes(bytes_), pixelsWide(pixelsWide_), pixelsHigh(pixelsHigh_)
{
    /* NB: don't free the image bytes in destructor as we assume they are freed elsewhere. */

    bytesPerPixel = (bitsPerPixel_ / 8);
    bytesPerRow = (bytesPerPixel * pixelsWide);
}


Color3 ImageTexture::value(double u, double v, Point3 *hitPt)
{
    if (!bytes) return color3(1, 1, 0);

    u = clamp(u, 0, 1);
    v = 1.0 - clamp(v, 0, 1); // Image is flipped!

    size_t i = (size_t)(u * pixelsWide);
    size_t j = (size_t)(v * pixelsHigh);

    if (i >= pixelsWide) i = pixelsWide - 1;
    if (j >= pixelsHigh) j = pixelsHigh - 1;

    const double invMaxByte = 1.0 / 255.0;

    uint8_t *pixels = bytes + (j * bytesPerRow + i * bytesPerPixel);

    return color3(pixels[0] * invMaxByte, pixels[1] * invMaxByte, pixels[2] * invMaxByte);
}
