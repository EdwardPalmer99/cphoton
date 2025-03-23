/**
 * @file ImageTexture.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Texture.hpp"
#include <cstdint>

class ImageTexture : public Texture
{
public:
    ImageTexture() = delete;
    ImageTexture(uint8_t *bytes, size_t pixelsWide, size_t pixelsHigh, size_t bitsPerPixel);

    Color3 value(double u, double v, Point3 *hitPt) override;

protected:
    uint8_t *bytes{nullptr};
    size_t pixelsWide;
    size_t pixelsHigh;
    size_t bytesPerPixel;
    size_t bytesPerRow;
};