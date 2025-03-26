/**
 * @file Cube.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Primitive.hpp"
#include "engine/materials/Material.hpp"
#include "utility/Matrix3.hpp"
#include "utility/Vector3.hpp"

class Cube : public Primitive
{
public:
    Cube() = delete;
    Cube(Point3 center_, Vector3 rotAngles_, double length_, std::shared_ptr<Material> material_);
    ~Cube() override;

    bool hit(Ray &ray, Hit &hit, HitType type) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    Point3 center;
    Rotate3 rotationMatrix;
    double length;
};