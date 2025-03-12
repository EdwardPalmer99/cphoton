/**
 * @file CSGPrimitive.c
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "CSGNode.hpp"
#include "engine/Span.hpp"

extern "C"
{
#include "engine/HitRec.h"
#include "logger/Logger.h"
#include "utility/Utility.h"
}


CSGNode::CSGNode(Primitive *left_, Primitive *right_, CSGOperation operationType_)
    : Primitive(nullptr), left(left_), right(right_), operationType(operationType_)
{
}


CSGNode::~CSGNode()
{
    /* NB: responsible for calling destructor of left, right primitives (take ownership in the constructor). */
    if (left) delete left;
    if (right) delete right;
}


/**
 * Calculate the bounding box for a CSG primitive.
 *
 * We want to call this recursively. At the leaf nodes, we generate a bounding box which includes both primitives we are
 * going to be doing an operation with. We then pass this up the tree, creating a box which encompasses everything
 * inside.
 */
bool CSGNode::boundingBox(AABB *outputBox)
{
    AABB leftBox, rightBox;

    // Create from the primitive or CSG primitive's methods:
    (void)left->boundingBox(&leftBox); // NB: expect always a true return.
    (void)right->boundingBox(&rightBox);

    // Combine to generate large bounding box encompassing everything in the tree. This can be made more efficient!
    *outputBox = (leftBox + rightBox);
    return true;
}


/**
 *
 * @param primitive The CSG primitive
 * @param ray       The ray we are intersecting
 * @param tmin      Min time for intersection (usually zero)
 * @param tmax      Max time for intersection (usually infinity)
 * @return          A SpanList record pointer or NULL if no intersections
 */
bool CSGNode::computeIntersections(Ray *ray, double tmin, double tmax, Span::SpanList &result)
{
    // Tree this as a tree. We want to compute this on any children and work our way up.
    Span::SpanList leftIntervals, rightIntervals;

    // TODO: - can use bool results to calculate if we need to perform operation.
    (void)left->computeIntersections(ray, tmin, tmax, leftIntervals);
    (void)right->computeIntersections(ray, tmin, tmax, rightIntervals);

    // TODO: - refer to notes here https://www.doc.ic.ac.uk/~dfg/graphics/graphics2008/GraphicsSlides10.pdf
    // we can optimize for certain operations if empty list returned.

    int count = 0;

    switch (operationType)
    {
        case CSGDifference:
            count = Span::differenceSpanLists(leftIntervals, rightIntervals, result);
            break;
        case CSGUnion:
            count = Span::unionSpanLists(leftIntervals, rightIntervals, result);
            break;
        case CSGIntersection:
            count = Span::intersectionSpanLists(leftIntervals, rightIntervals, result);
            break;
        default:
            LogFailed("This CSG operation type has not been implemented.");
            break;
    }

    return (count > 0);
}


/**
 * Returns true if the ray intersects with the primitive in interval [tmin, tmax]. If we have a hit, we populate hte
 * HitRec structure.
 */
bool CSGNode::hit(Ray *ray, double tmin, double tmax, HitRec *hit)
{
    Span::SpanList hitTimes;

    if (!computeIntersections(ray, tmin, tmax, hitTimes))
    {
        return false; // Hit nothing.
    }

    // Already sorted so take first item in list. Possible that entry time will be negative in which case we take texit.
    // Find the first positive t (either entry or exit).
    const Span &front = hitTimes.front();

    if (front.entry.t >= tmin)
        *hit = front.entry;
    else
        *hit = front.exit;

    return true;
}
