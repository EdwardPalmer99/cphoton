//
//  Camera.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "engine/Camera.hpp"
#include <cmath>
#include <utility>

extern "C"
{
#include "utility/MathMacros.h"
}


Camera::Camera(double verticalFOV, double aspectRatio, double focalLength, double aperture, Point3 origin,
               Point3 target)
    : _origin(std::move(origin))
{
    // The viewport coordinates are in the range:
    const double viewportHeight = 2.0 * tand(verticalFOV / 2.0);
    const double viewportWidth = aspectRatio * viewportHeight;

    // w vector is in opporite direction to camera's direction!
    Vector3 vectorUp(0, 1, 0);

    _w = (origin - target).normalize();
    _u = vectorUp.cross(_w).normalize();
    _v = _w.cross(_u).normalize(); // TODO: - don't need to normalize this one...

    _horizontal = _u * (focalLength * viewportWidth);
    _vertical = _v * (focalLength * viewportHeight);

    // Lower left corner of focal plane. This is in-front of the camera (opposite
    // direction to w vector).
    Vector3 halfHorizontal = (_horizontal * 0.5);
    Vector3 halfVertical = (_vertical * 0.5);
    Vector3 focalLengthMultW = (_w * focalLength);

    _lowerLeftCorner = _origin - ((halfHorizontal + halfVertical) + focalLengthMultW);
    _lensRadius = aperture / 2.0;
}


Ray Camera::fireRay(double s, double t) const
{
    // Calculate offset due to non-zero aperture (defocus blur):
    Vector3 randomInDisk = (Vector3::randomUnitDiskVector() * _lensRadius);
    Vector3 offset = (_u * randomInDisk.x()) + (_v * randomInDisk.y());

    Point3 rayStart = _origin + offset;
    Point3 rayTarget = _lowerLeftCorner + (_horizontal * s) + (_vertical * t);

    Vector3 direction = rayTarget - rayStart;

    return Ray(std::move(rayStart), std::move(direction));
}
