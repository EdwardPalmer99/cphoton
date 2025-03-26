/**
 * @file Ray.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Ray.hpp"
#include <utility>

Ray::Ray(Point3 origin, Vector3 direction) : _origin(std::move(origin)), _direction(std::move(direction))
{
}

Point3 Ray::pointAtTime(double t) const
{
    return (_origin + _direction * t);
}
