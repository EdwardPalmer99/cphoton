//
//  Ray.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "Ray.h"


Ray makeRay(Point3 origin, Vector3 direction)
{
    Ray ray = {.origin = origin, .direction = direction};

    return ray;
}


Point3 pointAtTime(Ray *ray, double t)
{
    return addVectors(ray->origin, scaleVector(ray->direction, t));
}
