//
//  Scene.c
//  CPhoton
//
//  Created by Edward on 28/01/2023.
//

#include "Scene.h"

static inline bool addSceneObject(Scene *scene, Primitive *object);
static inline void destructScene(Scene *scene);
static inline void markAsFinished(Scene *scene);

Scene *makeScene(void)
{
    Scene *scene = malloc(sizeof(Scene));
    if (!scene)
        return NULL;

    scene->numObjects = 0;
    scene->objectCapacity = 8;

    scene->objects = malloc(sizeof(Primitive *) * scene->objectCapacity);
    if (!scene->objects)
    {
        free(scene);
        return NULL;
    }

    scene->sceneNode = NULL; // Create when marked as finished.

    // Set "methods":
    scene->addObject = addSceneObject;
    scene->destructor = destructScene;
    scene->markAsFinished = markAsFinished;

    return scene;
}


/// Make sure that we don't continue to add objects after the scene has been
/// marked as completed!
static inline bool addSceneObject(Scene *scene, Primitive *object)
{
    if (!scene || !object || scene->sceneNode)
        return false;

    if (scene->numObjects >= scene->objectCapacity)
    {
        const size_t blockSize = scene->objectCapacity * 2 * sizeof(Primitive *);
        void *reallocPtr = realloc(scene->objects, blockSize);

        if (!reallocPtr)
            return false;

        scene->objects = reallocPtr;
        scene->objectCapacity *= 2;
    }

    scene->objects[scene->numObjects++] = object;
    return true;
}


/// Prevent more objects being added in the future. Also create our scene node
/// containing pointers to all objects in the scene.
static inline void markAsFinished(Scene *scene)
{
    if (!scene || scene->sceneNode || scene->numObjects < 1)
        return;

    if (scene->numObjects > scene->objectCapacity)
    {
        // Shrink to fit.
        scene->objectCapacity = scene->numObjects;
        scene->objects = realloc(scene->objects, sizeof(Primitive *) * scene->objectCapacity);
    }

    // Create BVH Node for all objects in the scene.
    scene->sceneNode = makeBVHNode(scene->objects, 0, scene->numObjects);
}


static inline void destructScene(Scene *scene)
{
    if (scene)
    {
        if (scene->objects)
        {
            if (scene->sceneNode)
            {
                // Scene node contains all objects in the scene so when we free
                // it we will free all other objects too.
                scene->sceneNode->destructor(scene->sceneNode);
            }
            else
            {
                // Need to manually free each object in the scene:
                for (int i = 0; i < scene->numObjects; i++)
                {
                    scene->objects[i]->destructor(scene->objects[i]);
                }
            }

            // Free array containing pointers to objects (doesn't free objects).
            free(scene->objects);
        }

        // Free memory for scene structure.
        free(scene);
    }
}
