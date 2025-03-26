/**
 * @file Primitive.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/AABB.hpp"
#include "engine/Hit.hpp"
#include "engine/Ray.hpp"
#include "engine/Span.hpp"
#include "engine/materials/Material.hpp"
#include "utility/Matrix3.hpp"
#include "utility/Vector3.hpp"
#include <memory>

/** Base object class. */
class Primitive
{
public:
    Primitive() = delete;

    virtual ~Primitive() = default;

    using Time = double;

    enum HitType
    {
        /* Return entry hit */
        Entry = 1,

        /* Return exit hit */
        Exit = 2
    };

    /* Returns a particular hit (ignores hit time which may be negative) */
    virtual bool hit(Ray &ray, Hit &hit, HitType type = Entry);

    /* Returns the closest hit in range (tmin, tmax) */
    virtual bool hit(Ray &ray, Time tmin, Time tmax, Hit &hit);

    /* Returns the (entry, exit) hit span. Requires at least one hit to be in range (tmin, tmax) */
    virtual bool hit(Ray &ray, Time tmin, Time tmax, Span::SpanList &result);

    /** On success, returns true and populates bounding box structure. */
    virtual bool boundingBox(AABB *boundingBox) = 0;

protected:
    Primitive(std::shared_ptr<Material> material_);

    /** Object material. */
    std::shared_ptr<Material> material{nullptr};
};

bool intersectionWithPlane(Point3 p0, Vector3 n, Ray &ray, double *hitTime);

Ray transformRay(Ray &ray, Point3 center, Rotate3 *rotation);

bool solveQuadratic(double a, double b, double c, double *t1, double *t2);
