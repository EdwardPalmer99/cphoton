//
//  MengerCube.h
//  GraphicsViewer
//
//  Created by Edward on 17/01/2023.
//

#ifndef MengerCube_h
#define MengerCube_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Vector3.h"
#include "Scene.h"
#include "Primitive.h"

Primitive *makeMengerSponge(int8_t n, Point3 center, double sideLength, Material *material);

#endif /* MengerCube_h */
