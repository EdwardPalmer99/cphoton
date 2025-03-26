/**
 * @file Sphere.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Sphere.hpp"
#include <cmath>

Sphere::Sphere(Point3 center_, double radius_, std::shared_ptr<Material> material_)
    : Primitive(material_), center(center_), radius(radius_)
{
}


bool Sphere::hit(Ray &ray, Hit &hit, HitType type)
{
    // ray origin 	 = O
    // ray direction = d
    // sphere center = C
    // sphere radius = r
    //
    // ray = O + t * d
    //
    // Solve:
    // (ray - C)^2 = r^2
    //
    // t^2*(d.d) + 2t*d.(O - C) + (O - C).(O - C) - r^2 = 0
    //
    // Quadratic formula:
    // t = [-B +/- sqrt(B^2 - 4 * AC)] / 2A
    //
    // A = d.d
    // B = 2d.(O - C)
    // C = (O - C).(O - C) - r^2

    Vector3 rayOriginMinusCenter = (ray.origin() - center);

    const double quadA = ray.direction().dot(ray.direction());
    const double quadB = 2.0 * (ray.direction().dot(rayOriginMinusCenter));
    const double quadC = rayOriginMinusCenter.dot(rayOriginMinusCenter) - radius * radius;

    double t1, t2;

    if (!solveQuadratic(quadA, quadB, quadC, &t1, &t2))
    {
        return false; // No hits.
    }

    double hitTime = ((type == Entry) ? t1 : t2);

    Point3 hitPoint = ray.pointAtTime(hitTime);

    // Compute the normal vector:
    Vector3 outwardNormal = (hitPoint - center) * (1.0 / radius);

    // Are we hitting the outside surface or are we hitting the inside?
    const bool frontFace = (ray.direction().dot(outwardNormal) < 0.0);

    hit.frontFace = frontFace;
    hit.t = hitTime;
    hit.hitPt = hitPoint;
    hit.normal = frontFace ? outwardNormal : -outwardNormal;
    hit.material = material.get();

    // Calculate the U, V texture coordinates:
    setSphereUV(&hit.normal, &hit.u, &hit.v);
    return true;
}


bool Sphere::boundingBox(AABB *boundingBox)
{
    Point3 min(center.x() - radius, center.y() - radius, center.z() - radius);
    Point3 max(center.x() + radius, center.y() + radius, center.z() + radius);
    *boundingBox = AABB(min, max);
    return true;
}


/// Calculate the texture coordinates which are in range [0, 1] using the outward
/// normal calculated from the hit. We are using spherical polar coordinates with
/// theta being the angle from the +y axis and phi being the anticlockwise angle
/// starting from the +x axis.
void setSphereUV(Vector3 *outwardNormal, double *u, double *v)
{
    const double theta = acos(-outwardNormal->y());
    const double phi = atan2(-outwardNormal->z(), outwardNormal->x()) + M_PI;

    *u = phi / (2.0 * M_PI);
    *v = theta / M_PI;
}
