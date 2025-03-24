/**
 * @file BVHNode.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "Primitive.hpp"

class BVHNode : public Primitive
{
public:
    BVHNode() = delete;
    BVHNode(Primitive **objects, int start, int end);
    ~BVHNode() override;

    bool hit(Ray *ray, double tmin, double tmax, Hit *hit) override;

    bool boundingBox(AABB *boundingBox) override;

protected:
    AABB box;
    Primitive *left{nullptr};
    Primitive *right{nullptr};
};
