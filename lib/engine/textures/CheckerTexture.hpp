/**
 * @file CheckerTexture.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Texture.hpp"
#include <memory>

/* A chessboard of two repeating textures */
class CheckerTexture : public Texture
{
public:
    CheckerTexture() = delete;
    CheckerTexture(std::shared_ptr<Texture> odd, std::shared_ptr<Texture> even);

    Color3 value(double u, double v, Point3 *hitPt) override;

protected:
    std::shared_ptr<Texture> odd{nullptr};
    std::shared_ptr<Texture> even{nullptr};
};