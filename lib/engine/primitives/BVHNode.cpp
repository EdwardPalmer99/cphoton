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

int compareBoundingBoxes(AABB *boxA, AABB *boxB, int axis);
int boxComparatorX(const void *ptr1, const void *ptr2);
int boxComparatorY(const void *ptr1, const void *ptr2);
int boxComparatorZ(const void *ptr1, const void *ptr2);


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

    box = boxLeft + boxRight;
}


BVHNode::~BVHNode()
{
    if (left) delete left;
    if (right) delete right;
}


bool BVHNode::hit(Ray *ray, double tmin, double tmax, HitRec *hit)
{
    if (!box.hit(ray, tmin, tmax)) return false;

    bool hitLeft = left && left->hit(ray, tmin, tmax, hit);
    bool hitRight = right && right->hit(ray, tmin, (hitLeft ? hit->t : tmax), hit);

    return (hitLeft || hitRight);
}


bool BVHNode::boundingBox(AABB *outputBox)
{
    *outputBox = box;
    return true;
}

int compareBoundingBoxes(AABB *boxA, AABB *boxB, int axis)
{
    double minA, minB;

    if (axis == 0)
    {
        minA = boxA->minPt().x;
        minB = boxB->minPt().x;
    }
    else if (axis == 1)
    {
        minA = boxA->minPt().y;
        minB = boxB->minPt().y;
    }
    else
    {
        minA = boxA->minPt().z;
        minB = boxB->minPt().z;
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
