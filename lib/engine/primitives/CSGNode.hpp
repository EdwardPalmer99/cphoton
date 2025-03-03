/**
 * @file CSGPrimitive.h
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */


#pragma once
#include "Primitive.hpp"

extern "C"
{
#include "engine/HitRec.h"
}

class CSGNode : public Primitive
{
public:
    enum CSGOperation
    {
        CSGUnion,       /* Merger of two objects into one. */
        CSGDifference,  /* Subtraction of one object from another. */
        CSGIntersection /* Portion common to both objects. */
    };

    CSGNode() = delete;
    CSGNode(Primitive *left_, Primitive *right_, CSGOperation operationType_);
    ~CSGNode() override;

    bool hit(Ray *ray, double tmin, double tmax, HitRec *hit) override;

    bool boundingBox(AABB *boundingBox) override;

    bool computeIntersections(Ray *ray, double tmin, double tmax, SpanList *result) override;

protected:
    Primitive *left;
    Primitive *right;

    /** Operation type. */
    CSGOperation operationType;
};
