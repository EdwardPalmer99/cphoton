//
//  Fractals3D.h
//  CPhoton
//
//  Created by Edward on 05/02/2023.
//

#ifndef Fractals3D_h
#define Fractals3D_h

#include "Camera.h"
#include "HitRec.h"
#include "Material.h"
#include "PPMWriter.h"
#include "Primitive.h"
#include "Ray.h"
#include <stdlib.h>

struct fractal_t;

struct primitive_t *makeMandelbulbFractal(Point3 center, Material *material);
struct primitive_t *makeQuaternionFractal(Point3 center, Material *material);

#endif /* Fractals3D_h */
