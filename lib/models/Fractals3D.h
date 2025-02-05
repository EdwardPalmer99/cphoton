//
//  Fractals3D.h
//  CPhoton
//
//  Created by Edward on 05/02/2023.
//

#ifndef Fractals3D_h
#define Fractals3D_h

#include "engine/Camera.h"
#include "engine/HitRec.h"
#include "engine/Material.h"
#include "engine/Primitive.h"
#include "engine/Ray.h"
#include "utility/PPMWriter.h"
#include <stdlib.h>

struct fractal_t;

struct primitive_t *makeMandelbulbFractal(Point3 center, Material *material);
struct primitive_t *makeQuaternionFractal(Point3 center, Material *material);

#endif /* Fractals3D_h */
