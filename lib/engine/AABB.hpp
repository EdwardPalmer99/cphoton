/**
 * @file AABB.hpp
 * @author Edward Palmer
 * @date 2025-03-03
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/Ray.hpp"
#include "utility/Vector3.hpp"

/**
 * Axis-aligned bounding box.
 */
class AABB
{
public:
    AABB();
    AABB(Point3 min, Point3 max);

    /** Resets bounding box. */
    void reset();

    /** Adds new point to bounding box. Recalculates size. */
    void addPoint(Point3 pt);

    /* Add bounding boxes */
    AABB operator+(const AABB &other);

    constexpr Point3 &minPt()
    {
        return min;
    }

    constexpr Point3 &maxPt()
    {
        return max;
    }

    constexpr const Point3 &minPt() const
    {
        return min;
    }

    constexpr const Point3 &maxPt() const
    {
        return max;
    }

    /** Returns true if box is hit by ray in range [tmin, tmax]. */
    bool hit(Ray &ray, double tmin, double tmax);

protected:
    /** Adds two bounding boxes and returns the result. */
    static AABB addBoundingBoxes(const AABB &box0, const AABB &box1);

    Point3 min, max;
};