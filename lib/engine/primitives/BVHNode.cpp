/**
 * @file BVHNode.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "BVHNode.hpp"
#include <stdexcept>

extern "C"
{
#include "utility/Utility.h"
}

#define swap(val1, val2)                                                                                               \
    ({                                                                                                                 \
        double temp = (val1);                                                                                          \
        (val1) = (val2);                                                                                               \
        (val2) = temp;                                                                                                 \
    })

AABB addBoundingBoxes(AABB box0, AABB box1);
int compareBoundingBoxes(AABB *boxA, AABB *boxB, int axis);
int boxComparatorX(const void *ptr1, const void *ptr2);
int boxComparatorY(const void *ptr1, const void *ptr2);
int boxComparatorZ(const void *ptr1, const void *ptr2);
bool isAABBHit(AABB *aabb, Ray *ray, double tmin, double tmax);


BVHNode::BVHNode(Primitive **objects, int start, int end) : Primitive(nullptr)
{
    const int objectSpan = (end - start);
    const int axis = randomInt(0, 2); // TODO: - split about largest axis.

    enum
    {
        kAxisX = 0,
        kAxisY = 1,
        kAxisZ = 2
    };

    int (*comparator)(const void *ptr1, const void *ptr2);

    comparator = (axis == kAxisX ? boxComparatorX : (axis == kAxisY ? boxComparatorY : boxComparatorZ));

    if (objectSpan == 1)
    {
        left = objects[start];
        right = nullptr;

        left->boundingBox(&box); // Special case!
        return;
    }
    else if (objectSpan == 2)
    {
        if (comparator(&objects[start], &objects[start + 1]) <= 0)
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else
    {
        qsort(objects + start, objectSpan, sizeof(Primitive *), comparator);

        int mid = start + objectSpan / 2;

        left = new BVHNode(objects, start, mid);
        right = new BVHNode(objects, mid, end);
    }

    // Calculate the node's bounding box (for objectSpan >= 2):
    AABB boxLeft, boxRight;

    if (!left->boundingBox(&boxLeft) || !right->boundingBox(&boxRight))
    {
        throw std::runtime_error("unable to add bounding boxes");
    }

    box = addBoundingBoxes(boxLeft, boxRight);
}


BVHNode::~BVHNode()
{
    if (left) delete left;
    if (right) delete right;
}


bool BVHNode::hit(Ray *ray, double tmin, double tmax, HitRec *hit)
{
    if (!isAABBHit(&box, ray, tmin, tmax)) return false;

    bool hitLeft = left && left->hit(ray, tmin, tmax, hit);
    bool hitRight = right && right->hit(ray, tmin, (hitLeft ? hit->t : tmax), hit);

    return (hitLeft || hitRight);
}


bool BVHNode::boundingBox(AABB *outputBox)
{
    *outputBox = box;
    return true;
}


bool isAABBHit(AABB *aabb, Ray *ray, double tmin, double tmax)
{
    Vector3 minAABB = aabb->min;
    Vector3 maxAABB = aabb->max;
    Point3 origin = ray->origin;

    // Now test against x-direction.
    double invD = 1.0 / ray->direction.x;
    double t0 = (minAABB.x - origin.x) * invD;
    double t1 = (maxAABB.x - origin.x) * invD;

    if (invD < 0.0) swap(t0, t1);

    tmin = max(t0, tmin);
    tmax = min(t1, tmax);

    if (tmax <= tmin) return false;

    // Now test against y-direction.
    invD = 1.0 / ray->direction.y;
    t0 = (minAABB.y - origin.y) * invD;
    t1 = (maxAABB.y - origin.y) * invD;

    if (invD < 0.0) swap(t0, t1);

    tmin = max(t0, tmin);
    tmax = min(t1, tmax);

    if (tmax <= tmin) return false;

    // Now test against z-direction:
    invD = 1.0 / ray->direction.z;
    t0 = (minAABB.z - origin.z) * invD;
    t1 = (maxAABB.z - origin.z) * invD;

    if (invD < 0.0) swap(t0, t1);

    tmin = max(t0, tmin);
    tmax = min(t1, tmax);

    if (tmax <= tmin) return false;

    return true;
}


AABB addBoundingBoxes(AABB box0, AABB box1)
{
    Point3 newMin = point3(min(box0.min.x, box1.min.x), min(box0.min.y, box1.min.y), min(box0.min.z, box1.min.z));
    Point3 newMax = point3(max(box0.max.x, box1.max.x), max(box0.max.y, box1.max.y), max(box0.max.z, box1.max.z));

    AABB newBoundingBox = {.min = newMin, .max = newMax};

    return newBoundingBox;
}

int compareBoundingBoxes(AABB *boxA, AABB *boxB, int axis)
{
    double minA, minB;

    if (axis == 0)
    {
        minA = boxA->min.x;
        minB = boxB->min.x;
    }
    else if (axis == 1)
    {
        minA = boxA->min.y;
        minB = boxB->min.y;
    }
    else
    {
        minA = boxA->min.z;
        minB = boxB->min.z;
    }

    if (minA < minB)
        return (-1);
    else if (minA > minB)
        return 1;
    else
        return 0;
}


int boxComparatorX(const void *ptr1, const void *ptr2)
{
    Primitive *objectA = *(Primitive **)ptr1;
    Primitive *objectB = *(Primitive **)ptr2;

    AABB boxA, boxB;

    if (!objectA->boundingBox(&boxA) || !objectB->boundingBox(&boxB))
    {
        return 0;
    }

    return compareBoundingBoxes(&boxA, &boxB, 0);
}


int boxComparatorY(const void *ptr1, const void *ptr2)
{
    Primitive *objectA = *(Primitive **)ptr1;
    Primitive *objectB = *(Primitive **)ptr2;

    AABB boxA, boxB;

    if (!objectA->boundingBox(&boxA) || objectB->boundingBox(&boxB))
    {
        return 0;
    }

    return compareBoundingBoxes(&boxA, &boxB, 1);
}


int boxComparatorZ(const void *ptr1, const void *ptr2)
{
    Primitive *objectA = *(Primitive **)ptr1;
    Primitive *objectB = *(Primitive **)ptr2;

    AABB boxA, boxB;

    if (!objectA->boundingBox(&boxA) || !objectB->boundingBox(&boxB))
    {
        return 0;
    }

    return compareBoundingBoxes(&boxA, &boxB, 2);
}
