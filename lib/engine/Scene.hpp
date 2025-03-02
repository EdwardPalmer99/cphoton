/**
 * @file Scene.hpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/primitives/BVHNode.hpp"
#include "engine/primitives/Primitive.hpp"

#include <vector>

class Scene
{
public:
    ~Scene();

    /** Adds object to the scene and takes ownership of memory. */
    bool addObject(Primitive *object);

    /** Constructs and returns BVH node. */
    BVHNode *BVH();

protected:
    /** Stores pointers to each object required for BVH. */
    std::vector<Primitive *> objects;

    /** Constructed BVH node. */
    BVHNode *bvh{nullptr};
};