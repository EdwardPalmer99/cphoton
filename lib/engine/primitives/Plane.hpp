/**
 * @file Disc.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Primitive.hpp"

class Plane : public Primitive
{
public:
    Plane() = delete;
    Plane(Point3 p0_, Point3 normal_, std::shared_ptr<Material> material_);
    ~Plane() override = default;

    bool hit(Ray *ray, double tmin, double tmax, HitRec *hit) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    Point3 p0; // Point on the plane.
    Point3 normal;
};
