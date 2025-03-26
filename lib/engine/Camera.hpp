//
//  Camera.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#pragma once
#include "engine/Ray.hpp"
#include "utility/Vector3.hpp"

class Camera
{
public:
    Camera() = delete;
    Camera(double verticalFOV, double aspectRatio, double focalLength, double aperture, Point3 origin, Point3 target);

    /*
     * @brief Get a camera ray for a particular pixel. s, t are in range [0, 1).
     */
    Ray fireRay(double s, double t) const;

protected:
    Point3 _origin;
    Vector3 _u, _v, _w;
    Vector3 _horizontal;
    Vector3 _vertical;
    Vector3 _lowerLeftCorner;
    double _lensRadius;
};
