/**
 * @file Plane.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Plane.hpp"

Plane::Plane(Point3 p0_, Point3 normal_, std::shared_ptr<Material> material_)
    : Primitive(material_), p0(p0_), normal(normal_)
{
}


bool Plane::hit(Ray *ray, double tmin, double tmax, Hit *hit)
{
    double hitTime = 0.0;

    if (intersectionWithPlane(p0, normal, ray, &hitTime) && isValidIntersectionTime(hitTime, tmin, tmax))
    {
        Vector3 outwardNormal = normal;

        const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

        hit->frontFace = frontFace;
        hit->t = hitTime;
        hit->hitPt = ray->pointAtTime(hitTime);
        hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
        hit->material = material.get();

        hit->u = 0.0;
        hit->v = 0.0;

        return true;
    }

    return false;
}


bool Plane::boundingBox(AABB *outputBox)
{
    const double deltaR = 0.001;

    if (fabs(normal.x) == 1.0)
    {
        outputBox->minPt() = point3(p0.x - deltaR, -INFINITY, -INFINITY);
        outputBox->maxPt() = point3(p0.x + deltaR, INFINITY, INFINITY);
    }
    else if (fabs(normal.y) == 1.0)
    {
        outputBox->minPt() = point3(-INFINITY, p0.y - deltaR, -INFINITY);
        outputBox->maxPt() = point3(INFINITY, p0.y + deltaR, INFINITY);
    }
    else if (fabs(normal.z) == 1.0)
    {
        outputBox->minPt() = point3(-INFINITY, -INFINITY, p0.z - deltaR);
        outputBox->maxPt() = point3(INFINITY, INFINITY, p0.z + deltaR);
    }
    else
    {
        outputBox->minPt() = point3(-INFINITY, -INFINITY, -INFINITY);
        outputBox->maxPt() = point3(INFINITY, INFINITY, INFINITY);
    }

    return true;
}
