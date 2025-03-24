/**
 * @file Primitive.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

extern "C"
{
#include "utility/Matrix3.h"
#include "utility/Vector3.h"
}

#include "CSGNodeInterface.hpp"
#include "engine/AABB.hpp"
#include "engine/Hit.hpp"
#include "engine/Ray.hpp"
#include "engine/Span.hpp"
#include "engine/materials/Material.hpp"
#include <memory>

/** Base object class. */
class Primitive : public CSGNodeInterface
{
public:
    Primitive() = delete;

    using Time = double;

    /** On success, returns true and populates hit structure if hit in interval [tmin, tmax]. */
    virtual bool hit(Ray &ray, Time tmin, Time tmax, Hit &hit) = 0;

    /** Optional. Raises an error if not overriden. */
    virtual bool computeIntersections(Ray &ray, double tmin, double tmax, Span::SpanList &result);

    /** On success, returns true and populates bounding box structure. */
    virtual bool boundingBox(AABB *boundingBox) = 0;

protected:
    Primitive(std::shared_ptr<Material> material_);

    /** Object material. */
    std::shared_ptr<Material> material{nullptr};
};

bool isValidIntersectionTime(double hitTime, double tmin, double tmax);
bool intersectionWithPlane(Point3 p0, Vector3 n, Ray &ray, double *hitTime);

Ray transformRay(Ray &ray, Point3 center, Rotate3 *rotation);

bool solveQuadratic(double a, double b, double c, double *t1, double *t2);
