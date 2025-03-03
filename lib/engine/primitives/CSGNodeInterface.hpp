/**
 * @file CSGNodeInterface.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

extern "C"
{
#include "engine/Ray.h"
#include "engine/SpanList.h"
}

/**
 * Interface to be implemented by primitives which are capable of CSG operations.
 */
class CSGNodeInterface
{
public:
    /**
     * @brief Populates the SpanList structure with all intersections between a ray and an object.
     */
    virtual bool computeIntersections(Ray *ray, double tmin, double tmax, SpanList *result) = 0;
    virtual ~CSGNodeInterface() = default;

protected:
    CSGNodeInterface() = default;
};