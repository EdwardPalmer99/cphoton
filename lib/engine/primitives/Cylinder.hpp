/**
 * @file Cylinder.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

extern "C"
{
#include "engine/Material.h"
#include "utility/Matrix3.h"
#include "utility/Vector3.h"
}

#include "Disc.hpp"
#include "Primitive.hpp"


class Cylinder : public Primitive
{
public:
    Cylinder() = delete;
    Cylinder(Point3 center_, Vector3 rotAngles_, double radius_, double height_, Material *material_);
    ~Cylinder() override;

    bool hit(Ray *ray, double tmin, double tmax, HitRec *hit) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    Disc topCap;
    Disc bottomCap;

    Point3 center;
    Rotate3 *rotationMatrix;
    double radius;
    double height;
};