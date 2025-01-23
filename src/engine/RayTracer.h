//
//  RayTracer.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#ifndef RayTracer_h
#define RayTracer_h

#include <stdlib.h>
#include "Ray.h"
#include "Camera.h"
#include "Vector3.h"
#include "Utility.h"
#include "Primitive.h"
#include "HitRec.h"
#include "PPMWriter.h"
#include "Scene.h"

#include <Cocoa/Cocoa.h>

PPMImage *renderScene(Scene *scene, Camera *camera, int imageWidth, int imageHeight, int samplesPerPixel, int maxDepth);

#endif /* RayTracer_h */
