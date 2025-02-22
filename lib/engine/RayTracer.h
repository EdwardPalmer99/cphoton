//
//  RayTracer.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#pragma once
#include "engine/Camera.h"
#include "engine/Scene.h"
#include "utility/PPMWriter.h"

PPMImage *renderScene(Scene *scene, Camera *camera);