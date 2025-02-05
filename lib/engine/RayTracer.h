//
//  RayTracer.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#ifndef RayTracer_h
#define RayTracer_h

#include "engine/Camera.h"
#include "engine/HitRec.h"
#include "engine/Primitive.h"
#include "engine/Ray.h"
#include "engine/Scene.h"
#include "utility/PPMWriter.h"
#include "utility/Utility.h"
#include "utility/Vector3.h"
#include <stdlib.h>

PPMImage *renderScene(Scene *scene, Camera *camera);

#endif /* RayTracer_h */
