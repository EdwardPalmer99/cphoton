/**
 * @file Disc.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Disc.hpp"

Disc::Disc(Point3 p0_, Point3 normal_, double radius_, std::shared_ptr<Material> material_)
    : Plane(p0_, normal_, material_), radius(radius_)
{
}


bool Disc::hit(Ray *ray, double tmin, double tmax, Hit *hit)
{
    double hitTime = 0.0;

    if (intersectionWithPlane(p0, normal, ray, &hitTime) && isValidIntersectionTime(hitTime, tmin, tmax))
    {
        // Check that hit point is inside disc radius:
        Point3 hitPoint = ray->pointAtTime(hitTime);

        Vector3 hitPointMinusCenter = subtractVectors(hitPoint, p0);

        if (dot(hitPointMinusCenter, hitPointMinusCenter) > (radius * radius))
        {
            return false;
        }

        Vector3 outwardNormal = normal;

        const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

        hit->frontFace = frontFace;
        hit->t = hitTime;
        hit->hitPt = hitPoint;
        hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
        hit->material = material.get();

        hit->u = 0.0;
        hit->v = 0.0;

        return true;
    }

    return false;
}


bool Disc::boundingBox(AABB *outputBox)
{
    const double deltaR = 0.001;

    if (fabs(normal.x) == 1.0)
    {
        outputBox->minPt() = point3(p0.x - deltaR, p0.y - radius, p0.z - radius);
        outputBox->maxPt() = point3(p0.x + deltaR, p0.y + radius, p0.z + radius);
    }
    else if (fabs(normal.y) == 1.0)
    {
        outputBox->minPt() = point3(p0.x - radius, p0.y - deltaR, p0.z - radius);
        outputBox->maxPt() = point3(p0.x + radius, p0.y + deltaR, p0.z + radius);
    }
    else if (fabs(normal.z) == 1.0)
    {
        outputBox->minPt() = point3(p0.x - radius, p0.y - radius, p0.z - deltaR);
        outputBox->maxPt() = point3(p0.x + radius, p0.y + radius, p0.z + deltaR);
    }
    else
    {
        outputBox->minPt() = point3(p0.x - radius, p0.y - radius, p0.z - radius);
        outputBox->maxPt() = point3(p0.x + radius, p0.y + radius, p0.z + radius);
    }

    return true;
}