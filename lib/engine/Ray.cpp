/**
 * @file Ray.cpp
 * @author Edward Palmer
 * @date 2025-03-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Ray.hpp"


Ray::Ray(Point3 origin_, Vector3 direction_) : origin(std::move(origin_)), direction(std::move(direction_))
{
}


Point3 Ray::pointAtTime(double t)
{
    return addVectors(origin, scaleVector(direction, t));
}
