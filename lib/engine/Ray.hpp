/**
 * @file Ray.hpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "utility/Vector3.hpp"

class Ray
{
public:
    /* Constructor */
    Ray() = default;
    Ray(Point3 origin, Vector3 direction);

    /* Returns the ray at a time t */
    Point3 pointAtTime(double t) const;

    /* Accessors */
    constexpr const Point3 origin() const
    {
        return _origin;
    }

    constexpr Point3 &origin()
    {
        return _origin;
    }

    constexpr const Vector3 &direction() const
    {
        return _direction;
    }

    constexpr Vector3 &direction()
    {
        return _direction;
    }

protected:
    Point3 _origin;
    Vector3 _direction;
};