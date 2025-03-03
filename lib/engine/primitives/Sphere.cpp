/**
 * @file Sphere.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Sphere.hpp"

Sphere::Sphere(Point3 center_, double radius_, Material *material_)
    : Primitive(material_), center(center_), radius(radius_)
{
}


bool Sphere::hit(Ray *ray, double tmin, double tmax, HitRec *hit)
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

    Vector3 rayOriginMinusCenter = subtractVectors(ray->origin, center);

    const double quadA = dot(ray->direction, ray->direction);
    const double quadB = 2.0 * dot(ray->direction, rayOriginMinusCenter);
    const double quadC = dot(rayOriginMinusCenter, rayOriginMinusCenter) - radius * radius;

    double t1, t2;

    if (!solveQuadratic(quadA, quadB, quadC, &t1, &t2)) return false;

    double hitTime;

    if (isValidIntersectionTime(t1, tmin, tmax))
        hitTime = t1;
    else if (isValidIntersectionTime(t2, tmin, tmax))
        hitTime = t2;
    else
        return false;

    Point3 hitPoint = pointAtTime(ray, hitTime);

    // Compute the normal vector:
    Vector3 outwardNormal = scaleVector(subtractVectors(hitPoint, center), 1.0 / radius);

    // Are we hitting the outside surface or are we hitting the inside?
    const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

    hit->frontFace = frontFace;
    hit->t = hitTime;
    hit->hitPt = hitPoint;
    hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
    hit->material = material;

    // Calculate the U, V texture coordinates:
    setSphereUV(&hit->normal, &hit->u, &hit->v);
    return true;
}


bool Sphere::boundingBox(AABB *boundingBox)
{
    boundingBox->max = point3(center.x + radius, center.y + radius, center.z + radius);
    boundingBox->min = point3(center.x - radius, center.y - radius, center.z - radius);

    return true;
}


bool Sphere::computeIntersections(Ray *ray, double tmin, double tmax, SpanList *result)
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

    Vector3 rayOriginMinusCenter = subtractVectors(ray->origin, center);

    const double quadA = dot(ray->direction, ray->direction);
    const double quadB = 2.0 * dot(ray->direction, rayOriginMinusCenter);
    const double quadC = dot(rayOriginMinusCenter, rayOriginMinusCenter) - radius * radius;

    double t1, t2; // t1 < t2

    // Case: no intersections.
    if (!solveQuadratic(quadA, quadB, quadC, &t1, &t2))
    {
        return false;
    }

    bool t1Valid = isValidIntersectionTime(t1, tmin, tmax);
    bool t2Valid = isValidIntersectionTime(t2, tmin, tmax);

    // Case: t1, t2 negative --> intersected behind the camera (IGNORE).
    if (!t1Valid && !t2Valid)
    {
        return false;
    }

    // TODO: - Only need material, t1, t2, normals.

    /* Compute intersection t1 (if t1 < 0 --> camera inside object) */
    {
        Point3 hitPoint = pointAtTime(ray, t1);

        Vector3 outwardNormal = scaleVector(subtractVectors(hitPoint, center), 1.0 / radius);

        // Are we hitting the outside surface or are we hitting the inside?
        const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

        result->n = 1;
        result->intervals[0].entry.frontFace = frontFace;
        result->intervals[0].entry.t = t1;
        result->intervals[0].entry.hitPt = hitPoint;
        result->intervals[0].entry.normal = frontFace ? outwardNormal : flipVector(outwardNormal);
        result->intervals[0].entry.material = material;
    }

    /* Compute intersection t2 */
    {
        Point3 hitPoint = pointAtTime(ray, t2);

        Vector3 outwardNormal = scaleVector(subtractVectors(hitPoint, center), 1.0 / radius);

        // Are we hitting the outside surface or are we hitting the inside?
        const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

        result->n = 1;
        result->intervals[0].exit.frontFace = frontFace;
        result->intervals[0].exit.t = t2;
        result->intervals[0].exit.hitPt = hitPoint;
        result->intervals[0].exit.normal = frontFace ? outwardNormal : flipVector(outwardNormal);
        result->intervals[0].exit.material = material;
    }

    return true;
}


/// Calculate the texture coordinates which are in range [0, 1] using the outward
/// normal calculated from the hit. We are using spherical polar coordinates with
/// theta being the angle from the +y axis and phi being the anticlockwise angle
/// starting from the +x axis.
void setSphereUV(Vector3 *outwardNormal, double *u, double *v)
{
    const double theta = acos(-outwardNormal->y);
    const double phi = atan2(-outwardNormal->z, outwardNormal->x) + M_PI;

    *u = phi / (2.0 * M_PI);
    *v = theta / M_PI;
}
