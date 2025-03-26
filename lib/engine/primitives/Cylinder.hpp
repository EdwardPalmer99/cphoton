/**
 * @file Cylinder.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Disc.hpp"
#include "Primitive.hpp"
#include "engine/materials/Material.hpp"
#include "utility/Matrix3.hpp"
#include "utility/Vector3.hpp"


class Cylinder : public Primitive
{
public:
    Cylinder() = delete;
    Cylinder(Point3 center_, Vector3 rotAngles_, double radius_, double height_, std::shared_ptr<Material> material_);

    bool hit(Ray &ray, Time tmin, Time tmax, Hit &hit) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    Disc topCap;
    Disc bottomCap;

    Point3 center;
    Rotate3 rotationMatrix;
    double radius;
    double height;
};