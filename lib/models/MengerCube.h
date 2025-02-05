//
//  MengerCube.h
//  GraphicsViewer
//
//  Created by Edward on 17/01/2023.
//

#ifndef MengerCube_h
#define MengerCube_h

#include "engine/Primitive.h"
#include "engine/Scene.h"
#include "utility/Vector3.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Primitive *makeMengerSponge(int8_t n, Point3 center, double sideLength, Material *material);

#endif /* MengerCube_h */
