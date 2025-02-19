//
//  Camera.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "engine/Camera.h"
#include "utility/Utility.h"

Camera makeCamera(double verticalFOV, double aspectRatio, double focalLength, double aperture, Point3 origin,
                  Point3 target)
{
    // The viewport coordinates are in the range:
    const double viewportHeight = 2.0 * tand(verticalFOV / 2.0);
    const double viewportWidth = aspectRatio * viewportHeight;

    // w vector is in opporite direction to camera's direction!
    Vector3 vectorUp = vector3(0, 1, 0);

    Vector3 w = unitVector(makeVectorFromPoints(origin, target));
    Vector3 u = unitVector(cross(vectorUp, w));
    Vector3 v = unitVector(cross(w, u)); // TODO: - don't need to normalize this one...

    Vector3 horizontal = scaleVector(u, focalLength * viewportWidth);
    Vector3 vertical = scaleVector(v, focalLength * viewportHeight);

    // Lower left corner of focal plane. This is in-front of the camera (opposite
    // direction to w vector).
    Vector3 halfHorizontal = scaleVector(horizontal, 0.5);
    Vector3 halfVertical = scaleVector(vertical, 0.5);
    Vector3 focalLengthMultW = scaleVector(w, focalLength);

    Vector3 lowerLeftCorner =
        subtractVectors(origin, addVectors(addVectors(halfHorizontal, halfVertical), focalLengthMultW));

    Camera camera = {
        .origin = origin,
        .u = u,
        .v = v,
        .w = w,
        .lowerLeftCorner = lowerLeftCorner,
        .horizontal = horizontal,
        .vertical = vertical,
        .lensRadius = aperture / 2.0,
    };

    return camera;
}


/// Get a camera ray for a particular pixel. s, t are in range [0, 1).
Ray getRay(Camera *camera, double s, double t)
{
    // Calculate offset due to non-zero aperature (defocus blur):
    Vector3 randomInDisk = scaleVector(randomInUnitDisk(), camera->lensRadius);
    Vector3 offset = addVectors(scaleVector(camera->u, randomInDisk.x), scaleVector(camera->v, randomInDisk.y));

    Point3 rayStart = addVectors(camera->origin, offset);

    Point3 rayTarget = addVectors(addVectors(camera->lowerLeftCorner, scaleVector(camera->horizontal, s)),
                                  scaleVector(camera->vertical, t));

    Vector3 direction = subtractVectors(rayTarget, rayStart);

    return makeRay(rayStart, direction);
}
