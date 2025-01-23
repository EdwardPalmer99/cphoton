//
//  Matrix3.h
//  CPhoton
//
//  Created by Edward on 23/01/2023.
//

#ifndef Matrix3_h
#define Matrix3_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Vector3.h"
#include "Utility.h"

struct rotate3_t;

typedef struct rotate3_t Rotate3;

Rotate3 *makeRotate3(Vector3 rotationAngles);

Vector3 rotation(Vector3 v, Rotate3 *matrices);
Vector3 inverseRotation(Vector3, Rotate3 *matrices);

#endif /* Matrix3_h */
