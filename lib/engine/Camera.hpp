//
//  Camera.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#pragma once
#include "engine/Ray.hpp"

extern "C"
{
#include "utility/Vector3.h"
}

class Camera
{
public:
    Camera() = delete;
    Camera(double verticalFOV_, double aspectRatio_, double focalLength_, double aperture_, Point3 origin_,
           Point3 target_);

    /*
     * @brief Get a camera ray for a particular pixel. s, t are in range [0, 1).
     */
    Ray fireRay(double s, double t);

protected:
    Point3 origin;
    Vector3 u, v, w;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 lowerLeftCorner;
    double lensRadius;
};
