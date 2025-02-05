//
//  Scene.h
//  CPhoton
//
//  Created by Edward on 28/01/2023.
//

#ifndef Scene_h
#define Scene_h

#include "engine/Primitive.h"

typedef struct scene_t Scene;

Scene *makeScene(void);

struct scene_t
{
    int numObjects, objectCapacity;

    Primitive **objects;
    Primitive *sceneNode;

    // "Methods".
    bool (*addObject)(Scene *scene, Primitive *object);
    void (*destructor)(Scene *scene);
    void (*markAsFinished)(Scene *scene);
};

#endif /* Scene_h */
