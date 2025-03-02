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
#include "engine/HitRec.h"
#include "engine/Material.h"
#include "engine/Ray.h"
#include "utility/Matrix3.h"
#include "utility/Vector3.h"
}

struct AABB // Axis-aligned Bounding box.
{
    Point3 min, max;
};

/** Base object class. */
class Primitive
{
public:
    Primitive() = delete;
    virtual ~Primitive();

    /** On success, returns true and populates hit structure if hit in interval [tmin, tmax]. */
    virtual bool hit(Ray *ray, double tmin, double tmax, HitRec *hit) = 0;

    /** On success, returns true and populates bounding box structure. */
    virtual bool boundingBox(AABB *boundingBox) = 0;

protected:
    Primitive(Material *material_);

    /** Object material. */
    Material *material{nullptr};
};


void resetBoundingBox(AABB *outputBox);
void addPointToBoundingBox(Point3 pt, AABB *outputBox);

bool isValidIntersectionTime(double hitTime, double tmin, double tmax);
bool intersectionWithPlane(Point3 p0, Vector3 n, Ray *ray, double *hitTime);

Ray transformRay(Ray *ray, Point3 center, Rotate3 *rotation);

bool solveQuadratic(double a, double b, double c, double *t1, double *t2);
