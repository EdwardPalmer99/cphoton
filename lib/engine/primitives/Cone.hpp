/**
 * @file Cone.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Disc.hpp"
#include "Primitive.hpp"

extern "C"
{
#include "utility/Matrix3.h"
#include "utility/Vector3.h"
}

class Cone : public Primitive
{
public:
    Cone() = delete;
    Cone(Point3 center_, Vector3 rotAngles_, double height_, std::shared_ptr<Material> material_);
    ~Cone() override;

    bool hit(Ray *ray, double tmin, double tmax, Hit *hit) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    Point3 center;
    double height;
    Disc base;
    Rotate3 *rotationMatrix;
};
