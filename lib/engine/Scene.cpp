/**
 * @file Scene.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Scene.hpp"


bool Scene::addObject(Primitive *object)
{
    // Do not add objects if null or already constructed.
    if (!object || bvh)
    {
        return false;
    }

    objects.push_back(object);
    return true;
}


BVHNode *Scene::BVH()
{
    // Return pointer to already constructed BVH.
    if (bvh)
    {
        return bvh;
    }

    // Not yet constructed. Check if we can construct.
    if (objects.size() < 1)
    {
        return nullptr;
    }

    // Construct.
    objects.shrink_to_fit();
    bvh = new BVHNode(objects.data(), 0, objects.size());

    // No longer require vector of pointers. The BVHNode when its destructor is called
    // will cleanup all memory since it takes ownership of primitives.
    objects.clear();

    return bvh;
}


Scene::~Scene()
{
    if (bvh)
    {
        // BVH will delete all objects in its destructor.
        delete bvh;
    }
    else
    {
        // Delete all objects in vector.
        for (auto *object : objects)
        {
            delete object;
        }
    }
}