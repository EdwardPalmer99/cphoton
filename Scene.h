//
//  Scene.h
//  CPhoton
//
//  Created by Edward on 28/01/2023.
//

#ifndef Scene_h
#define Scene_h

#include "Primitive.h"

typedef struct scene_t Scene;

Scene *makeScene(void);
Primitive *makeDarkKnightRoom(double length, double width, double height);

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
