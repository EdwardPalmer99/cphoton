//
//  PolyhedralModels.h
//  CPhoton
//
//  Created by Edward on 30/01/2023.
//

#ifndef PolyhedralModels_h
#define PolyhedralModels_h

#include "engine/Material.h"
#include "engine/Primitive.h"
#include "engine/Texture.h"
#include "utility/Matrix3.h"
#include "utility/Utility.h"
#include "utility/Vector3.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Primitive *makeTetrahedron(Point3 baseCenter, Vector3 rotationAngles, double sideLength, Material *material);
Primitive *makeStellatedOctahedron(Point3 center, double sideLength, Material *materialTop, Material *materialBottom);

#endif /* PolyhedralModels_h */
