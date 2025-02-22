//
//  Camera.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#pragma once
#include "utility/Vector3.h"
#include "engine/Ray.h"

typedef struct
{
    Point3 origin;
    Vector3 u, v, w;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 lowerLeftCorner;
    double lensRadius;
} Camera;


Camera makeCamera(double verticalFOV, double aspectRatio, double focalLength, double aperture, Point3 origin,
                  Point3 target);
Ray getRay(Camera *camera, double s, double t);
