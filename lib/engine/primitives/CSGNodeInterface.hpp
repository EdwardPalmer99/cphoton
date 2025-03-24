/**
 * @file CSGNodeInterface.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/Ray.hpp"
#include "engine/Span.hpp"

/**
 * Interface to be implemented by primitives which are capable of CSG operations.
 */
class CSGNodeInterface
{
public:
    using Time = double;

    /**
     * @brief Populates the SpanList structure with all intersections between a ray and an object.
     */
    virtual bool hit(Ray &ray, Time tmin, Time tmax, Span::SpanList &result) = 0;

    virtual ~CSGNodeInterface() = default;

protected:
    CSGNodeInterface() = default;
};