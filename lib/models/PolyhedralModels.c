//
//  PolyhedralModels.c
//  CPhoton
//
//  Created by Edward on 30/01/2023.
//

#include "PolyhedralModels.h"

Primitive *makeTetrahedron(Point3 baseCenter, Vector3 rotationAngles, double sideLength, Material *material)
{
    const double kHalfSideLen = 0.5 * sideLength;
    const double kSqrt3Over6 = sqrt(3.0) / 6.0;
    const double kSqrt3Over2 = sqrt(3.0) / 2.0;
    const double height = sqrt(2.0 / 3.0) * sideLength;

    Point3 v0 = point3(-kHalfSideLen, 0, +sideLength * kSqrt3Over6);
    Point3 v1 = point3(kHalfSideLen, 0, +sideLength * kSqrt3Over6);
    Point3 v2 = point3(0, 0, v1.z - sideLength * kSqrt3Over2);
    Point3 v3 = point3(0, height, 0);

    Rotate3 *matrices = makeRotate3(rotationAngles);
    if (matrices)
    {
        v0 = rotation(v0, matrices); // Add rotation.
        v1 = rotation(v1, matrices);
        v2 = rotation(v2, matrices);
        v3 = rotation(v3, matrices);

        free(matrices);
    }

    v0 = addVectors(v0, baseCenter); // Add translation.
    v1 = addVectors(v1, baseCenter);
    v2 = addVectors(v2, baseCenter);
    v3 = addVectors(v3, baseCenter);

    // Build faces:
    Primitive *base = makeTriangle(v1, v0, v2, material);
    Primitive *front = makeTriangle(v0, v1, v3, material);
    Primitive *left = makeTriangle(v2, v0, v3, material);
    Primitive *right = makeTriangle(v1, v2, v3, material);

    Primitive *faces[] = {base, front, left, right};
    Primitive *tetrahedron = makeBVHNode(faces, 0, 4);

    return tetrahedron;
}


Primitive *makeStellatedOctahedron(Point3 center, double sideLength, Material *materialTop, Material *materialBottom)
{
    // Build centered about y-axis:
    const double yOffset = (1.0 / sqrt(6.0)) * sideLength;
    Point3 bottomCenter = point3(center.x, center.y + yOffset, center.z);

    Primitive *top = makeTetrahedron(center, zeroVector(), sideLength, materialTop);
    Primitive *bottom = makeTetrahedron(bottomCenter, vector3(180, 0, 0), sideLength, materialBottom);

    Primitive *objects[] = {top, bottom};
    Primitive *stellatedOctahedron = makeBVHNode(objects, 0, 2);

    return stellatedOctahedron;
}
