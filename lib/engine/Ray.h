//
//  Ray.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#ifndef Ray_h
#define Ray_h

#include "Vector3.h"

typedef struct
{
    Point3 origin;
    Vector3 direction;
} Ray;


Ray makeRay(Point3 origin, Vector3 direction);
Point3 pointAtTime(Ray *ray, double t);

#endif /* Ray_h */
