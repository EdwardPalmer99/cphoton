//
//  Camera.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "engine/Camera.hpp"

extern "C"
{
#include "utility/MathMacros.h"
}


Camera::Camera(double verticalFOV_, double aspectRatio_, double focalLength_, double aperture_, Point3 origin_,
               Point3 target_)
    : origin(origin_)
{
    // The viewport coordinates are in the range:
    const double viewportHeight = 2.0 * tand(verticalFOV_ / 2.0);
    const double viewportWidth = aspectRatio_ * viewportHeight;

    // w vector is in opporite direction to camera's direction!
    Vector3 vectorUp = vector3(0, 1, 0);

    w = unitVector(makeVectorFromPoints(origin_, target_));
    u = unitVector(cross(vectorUp, w));
    v = unitVector(cross(w, u)); // TODO: - don't need to normalize this one...

    horizontal = scaleVector(u, focalLength_ * viewportWidth);
    vertical = scaleVector(v, focalLength_ * viewportHeight);

    // Lower left corner of focal plane. This is in-front of the camera (opposite
    // direction to w vector).
    Vector3 halfHorizontal = scaleVector(horizontal, 0.5);
    Vector3 halfVertical = scaleVector(vertical, 0.5);
    Vector3 focalLengthMultW = scaleVector(w, focalLength_);

    lowerLeftCorner = subtractVectors(origin_, addVectors(addVectors(halfHorizontal, halfVertical), focalLengthMultW));
    lensRadius = aperture_ / 2.0;
}


Ray Camera::fireRay(double s, double t)
{
    // Calculate offset due to non-zero aperature (defocus blur):
    Vector3 randomInDisk = scaleVector(randomInUnitDisk(), lensRadius);
    Vector3 offset = addVectors(scaleVector(u, randomInDisk.x), scaleVector(v, randomInDisk.y));

    Point3 rayStart = addVectors(origin, offset);

    Point3 rayTarget = addVectors(addVectors(lowerLeftCorner, scaleVector(horizontal, s)), scaleVector(vertical, t));

    Vector3 direction = subtractVectors(rayTarget, rayStart);

    return Ray(rayStart, direction);
}
