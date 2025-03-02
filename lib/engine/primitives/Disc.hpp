/**
 * @file Disc.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Plane.hpp"

class Disc : public Plane
{
public:
    Disc() = delete;
    Disc(Point3 p0_, Point3 normal_, double radius_, Material *material_);
    ~Disc() override = default;

    bool hit(Ray *ray, double tmin, double tmax, HitRec *hit) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    double radius;
};