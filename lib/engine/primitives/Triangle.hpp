/**
 * @file Triangle.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Primitive.hpp"
#include "engine/Hit.hpp"
#include "engine/materials/Material.hpp"

extern "C"
{
#include "utility/Vector3.h"
}


class Triangle : public Primitive
{
public:
    Triangle() = delete;
    Triangle(Point3 v0_, Point3 v1_, Point3 v2_, std::shared_ptr<Material> material_);
    ~Triangle() override = default;

    bool hit(Ray *ray, double tmin, double tmax, Hit *hit) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    Point3 v0, v1, v2;
    Vector3 normal;
};