/**
 * @file AABB.cpp
 * @author Edward Palmer
 * @date 2025-03-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "AABB.hpp"

#define swap(val1, val2)                                                                                               \
    ({                                                                                                                 \
        double temp = (val1);                                                                                          \
        (val1) = (val2);                                                                                               \
        (val2) = temp;                                                                                                 \
    })


AABB::AABB()
{
    reset();
}


AABB::AABB(Point3 min_, Point3 max_) : min(min_), max(max_)
{
}


void AABB::reset()
{
    min = point3(INFINITY, INFINITY, INFINITY);
    max = point3(-INFINITY, -INFINITY, -INFINITY);
}


void AABB::addPoint(Point3 pt)
{
    if (pt.x < min.x) min.x = pt.x;
    if (pt.x > max.x) max.x = pt.x;

    if (pt.y < min.y) min.y = pt.y;
    if (pt.y > max.y) max.y = pt.y;

    if (pt.z < min.z) min.z = pt.z;
    if (pt.z > max.z) max.z = pt.z;
}


AABB AABB::addBoundingBoxes(const AABB &box0, const AABB &box1)
{
    Point3 newMin =
        point3(std::min(box0.min.x, box1.min.x), std::min(box0.min.y, box1.min.y), std::min(box0.min.z, box1.min.z));
    Point3 newMax =
        point3(std::max(box0.max.x, box1.max.x), std::max(box0.max.y, box1.max.y), std::max(box0.max.z, box1.max.z));

    return AABB(newMin, newMax);
}


AABB AABB::operator+(const AABB &other)
{
    return addBoundingBoxes(*this, other);
}


bool AABB::hit(Ray &ray, double tmin, double tmax)
{
    Point3 origin = ray.origin;

    // Now test against x-direction.
    double invD = 1.0 / ray.direction.x;
    double t0 = (min.x - origin.x) * invD;
    double t1 = (max.x - origin.x) * invD;

    if (invD < 0.0) swap(t0, t1);

    tmin = std::max(t0, tmin);
    tmax = std::min(t1, tmax);

    if (tmax <= tmin) return false;

    // Now test against y-direction.
    invD = 1.0 / ray.direction.y;
    t0 = (min.y - origin.y) * invD;
    t1 = (max.y - origin.y) * invD;

    if (invD < 0.0) swap(t0, t1);

    tmin = std::max(t0, tmin);
    tmax = std::min(t1, tmax);

    if (tmax <= tmin) return false;

    // Now test against z-direction:
    invD = 1.0 / ray.direction.z;
    t0 = (min.z - origin.z) * invD;
    t1 = (max.z - origin.z) * invD;

    if (invD < 0.0) swap(t0, t1);

    tmin = std::max(t0, tmin);
    tmax = std::min(t1, tmax);

    if (tmax <= tmin) return false;

    return true;
}
