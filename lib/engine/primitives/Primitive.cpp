/**
 * @file Primitive.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Primitive.hpp"
#include <cmath>
#include <stdexcept>


Primitive::Primitive(std::shared_ptr<Material> material_) : material(material_)
{
}


bool Primitive::hit(Ray &ray, Hit &hit, HitType type)
{
    throw std::logic_error("Not implemented");
}


bool Primitive::hit(Ray &ray, Time tmin, Time tmax, Hit &hitRec)
{
    if (!hit(ray, hitRec, Entry))
    {
        return false; // No hits.
    }

    if (!hitRec.isValid(tmin, tmax))
    {
        (void)hit(ray, hitRec, Exit); // Try exit time (case: camera could be inside object).
        if (!hitRec.isValid(tmin, tmax))
        {
            return false;
        }
    }

    return true;
}


bool Primitive::hit(Ray &ray, Time tmin, Time tmax, Span::SpanList &result)
{
    /* Calculate entry hit */
    Hit entry;
    if (!hit(ray, entry, Entry))
    {
        return false; // Ray never intersected sphere.
    }

    /* Calculate exit hit (nb: if we hit on entry, we must hit on exit) */
    Hit exit;
    (void)hit(ray, exit, Exit);

    /*
     * NB: we allow 1 of the intersections to be invalid. i.e. Entry is behind camera but exit is ahead of camera
     * which would be the case where camera is inside object. This is fine because we will be performing CSG operations
     * and only need to check at the end whether the intersection is valid.
     */
    if (!entry.isValid(tmin, tmax) && !exit.isValid(tmin, tmax))
    {
        return false; // Full intersection behind camera --> ignore.
    }

    result.push_back(Span(entry, exit));
    return true;
}


/// Returns a positive value > 0 if a ray intersects with a plane. p0 is an
/// arbitrary point on the plane and n is the plane's normal vector.
bool intersectionWithPlane(Point3 p0, Vector3 n, Ray &ray, double *hitTime)
{
    // Eqn for line: p = origin + t * d
    // Plane: (p - p0).n = 0 where n is normal to plane and p0 is point on the plane.
    //
    // --> (origin - p0).n + t*(d.n) = 0
    // --> t = (p0 - origin).n / (d.n)
    const double directionDotN = ray.direction().dot(n);
    const double p0MinusOriginDotN = (p0 - ray.origin()).dot(n);

    // Line and plane are parallel --> no intersection point:
    if (directionDotN == 0) return false;

    // Negative intersect time --> intersects behind ray's position:
    const double intersectTime = (p0MinusOriginDotN / directionDotN);
    if (intersectTime < 0.0) return false;

    *hitTime = intersectTime;
    return true;
}


Ray transformRay(Ray &ray, Point3 center, Rotate3 *rotation)
{
    Vector3 newDir = rotation->undoRotation(ray.direction());
    Point3 newOrigin = rotation->undoRotation(ray.origin() - center);

    return Ray(newOrigin, newDir);
}


bool solveQuadratic(double a, double b, double c, double *t1, double *t2)
{
    const double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0) return 0; // No roots.

    const double sqrtDiscriminant = std::sqrt(discriminant);

    *t1 = (-b - sqrtDiscriminant) / (2 * a);
    *t2 = (-b + sqrtDiscriminant) / (2 * a);

    return true;
}
