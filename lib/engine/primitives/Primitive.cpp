/**
 * @file Primitive.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Primitive.hpp"

Primitive::Primitive(Material *material_) : material(material_)
{
    if (material)
    {
        material->incrementRefCounter(material);
    }
}


Primitive::~Primitive()
{
    if (material)
    {
        material->decrementRefCounter(material);
    }
}


void resetBoundingBox(AABB *outputBox)
{
    if (!outputBox) return;

    outputBox->min = point3(INFINITY, INFINITY, INFINITY);
    outputBox->max = point3(-INFINITY, -INFINITY, -INFINITY);
}


void addPointToBoundingBox(Point3 pt, AABB *outputBox)
{
    if (pt.x < outputBox->min.x) outputBox->min.x = pt.x;
    if (pt.x > outputBox->max.x) outputBox->max.x = pt.x;

    if (pt.y < outputBox->min.y) outputBox->min.y = pt.y;
    if (pt.y > outputBox->max.y) outputBox->max.y = pt.y;

    if (pt.z < outputBox->min.z) outputBox->min.z = pt.z;
    if (pt.z > outputBox->max.z) outputBox->max.z = pt.z;
}


bool isValidIntersectionTime(double hitTime, double tmin, double tmax)
{
    return (tmin < hitTime && hitTime < tmax);
}


/// Returns a positive value > 0 if a ray intersects with a plane. p0 is an
/// arbitrary point on the plane and n is the plane's normal vector.
bool intersectionWithPlane(Point3 p0, Vector3 n, Ray *ray, double *hitTime)
{
    // Eqn for line: p = origin + t * d
    // Plane: (p - p0).n = 0 where n is normal to plane and p0 is point on the plane.
    //
    // --> (origin - p0).n + t*(d.n) = 0
    // --> t = (p0 - origin).n / (d.n)
    const double directionDotN = dot(ray->direction, n);
    const double p0MinusOriginDotN = dot(subtractVectors(p0, ray->origin), n);

    // Line and plane are parallel --> no intersection point:
    if (directionDotN == 0) return false;

    // Negative intersect time --> intersects behind ray's position:
    const double intersectTime = (p0MinusOriginDotN / directionDotN);
    if (intersectTime < 0.0) return false;

    *hitTime = intersectTime;
    return true;
}


Ray transformRay(Ray *ray, Point3 center, Rotate3 *rotation)
{
    Vector3 newDir = inverseRotation(ray->direction, rotation);
    Point3 newOrigin = inverseRotation(subtractVectors(ray->origin, center), rotation);

    return makeRay(newOrigin, newDir);
}


bool solveQuadratic(double a, double b, double c, double *t1, double *t2)
{
    const double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0) return 0; // No roots.

    const double sqrtDiscriminant = sqrt(discriminant);

    *t1 = (-b - sqrtDiscriminant) / (2 * a);
    *t2 = (-b + sqrtDiscriminant) / (2 * a);

    return true;
}
