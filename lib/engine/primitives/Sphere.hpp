/**
 * @file Sphere.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */


#pragma once
#include "Primitive.hpp"
#include "engine/Ray.hpp"
#include "utility/Vector3.hpp"

class Sphere : public Primitive
{
public:
    Sphere() = delete;
    Sphere(Point3 center, double radius, std::shared_ptr<Material> material);

    /* Returns the entry or exit hit time */
    bool hit(Ray &ray, Hit &hit, HitType type) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    Point3 center;
    double radius;
};

void setSphereUV(Vector3 *outwardNormal, double *u, double *v);
