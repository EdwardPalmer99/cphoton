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
#include "utility/Matrix3.hpp"
#include "utility/Vector3.hpp"

class Cone : public Primitive
{
public:
    Cone() = delete;
    Cone(Point3 center_, Vector3 rotAngles_, double height_, std::shared_ptr<Material> material_);

    bool hit(Ray &ray, Time tmin, Time tmax, Hit &hit) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    Point3 center;
    double height;
    Disc base;
    Rotate3 rotationMatrix;
};
