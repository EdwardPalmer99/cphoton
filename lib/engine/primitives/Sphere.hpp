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

extern "C"
{
#include "engine/Ray.h"
#include "utility/Vector3.h"
}


class Sphere : public Primitive
{
public:
    Sphere() = delete;
    Sphere(Point3 center, double radius, Material *material);

    bool hit(Ray *ray, double tmin, double tmax, HitRec *hit) override;

    bool boundingBox(AABB *boundingBox) override;

    bool computeIntersections(Ray *ray, double tmin, double tmax, SpanList &result) override;

protected:
    Point3 center;
    double radius;
};

void setSphereUV(Vector3 *outwardNormal, double *u, double *v);
