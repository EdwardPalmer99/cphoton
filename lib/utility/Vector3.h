//
//  Vector3.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#ifndef Vector3_h
#define Vector3_h

#include "Utility.h"
#include <math.h>
#include <stdbool.h>

#define kZeroTolerance 1e-8

typedef struct
{
    union
    {
        double r, x;
    };
    union
    {
        double g, y;
    };
    union
    {
        double b, z;
    };
} Vector3, Point3, Color3;

double lengthSquared(Vector3 v);
double vectorLength(Vector3 v);
double dot(Vector3 u, Vector3 v);

Vector3 flipVector(Vector3 v);
Vector3 scaleVector(Vector3 v, const double scalar);
Vector3 unitVector(Vector3 v);
Vector3 cross(Vector3 u, Vector3 v);
Vector3 subtractVectors(Vector3 u, Vector3 v);
Vector3 addVectors(Vector3 u, Vector3 v);
Vector3 vector3(double x, double y, double z);
Vector3 makeVectorFromPoints(Point3 ptA, Point3 ptB);
Point3 point3(double x, double y, double z);
Color3 color3(double r, double g, double b);
Vector3 randomUnitSphereVector(void);
Vector3 randomUnitVector(void);
Vector3 randomInUnitDisk(void);
bool isNearlyZero(Vector3 v);
Color3 multiplyColors(Color3 color1, Color3 color2);
int randomInt(int min, int max);
Vector3 zeroVector(void);

#endif /* Vector3_h */
