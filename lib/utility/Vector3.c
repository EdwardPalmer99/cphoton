//
//  Vector3.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "utility/Vector3.h"
#include "utility/Utility.h"

Color3 color3(double r, double g, double b)
{
    Color3 color = {r, g, b};

    return color;
}


Point3 point3(double x, double y, double z)
{
    Point3 pt = {x, y, z};

    return pt;
}


Vector3 makeVectorFromPoints(Point3 ptA, Point3 ptB)
{
    return subtractVectors(ptA, ptB);
}


Vector3 vector3(double x, double y, double z)
{
    Vector3 v = {x, y, z};

    return v;
}


Vector3 addVectors(Vector3 u, Vector3 v)
{
    return vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}


Vector3 subtractVectors(Vector3 u, Vector3 v)
{
    return vector3(u.x - v.x, u.y - v.y, u.z - v.z);
}


double dot(Vector3 u, Vector3 v)
{
    return (u.x * v.x + u.y * v.y + u.z * v.z);
}


Vector3 cross(Vector3 u, Vector3 v)
{
    double iComponent = u.y * v.z - u.z * v.y;
    double jComponent = u.z * v.x - u.x * v.z;
    double kComponent = u.x * v.y - u.y * v.x;

    return vector3(iComponent, jComponent, kComponent);
}


double vectorLength(Vector3 v)
{
    return sqrt(lengthSquared(v));
}


Vector3 flipVector(Vector3 v)
{
    return vector3(-v.x, -v.y, -v.z);
}


Vector3 scaleVector(Vector3 v, const double scalar)
{
    return vector3(v.x * scalar, v.y * scalar, v.z * scalar);
}


Color3 multiplyColors(Color3 color1, Color3 color2)
{
    Color3 outputColor = {.r = color1.r * color2.r, .g = color1.g * color2.g, .b = color1.b * color2.b};

    return outputColor;
}


Vector3 unitVector(Vector3 v)
{
    const double vLength = vectorLength(v);

    return (vLength > 0.0) ? scaleVector(v, 1.0 / vLength) : v;
}


Vector3 randomVector(void)
{
    return vector3(randomDouble(), randomDouble(), randomDouble());
}


Vector3 randomVectorBetween(double min, double max)
{
    return vector3(randomDoubleRange(min, max), randomDoubleRange(min, max), randomDoubleRange(min, max));
}


double lengthSquared(Vector3 v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}


/// Returns a vector inside a unit sphere. Points are generated in the range
/// [-1, 1] for x, y, z. If the length squared is less than or equal to 1.0 then
/// it is inside the unit sphere.
Vector3 randomUnitSphereVector(void)
{
    Vector3 vectorOut;
    double vectorLengthSquared;

    do
    {
        vectorOut = randomVectorBetween(-1.0, 1.0);
        vectorLengthSquared = lengthSquared(vectorOut);
    } while (vectorLengthSquared >= 1.0);

    return vectorOut;
}


/// Generate a vector inside the unit sphere and then normalize.
Vector3 randomUnitVector(void)
{
    return unitVector(randomUnitSphereVector());
}


Vector3 randomInUnitDisk(void)
{
    Vector3 vectorOut;
    double vectorLengthSquared;

    do
    {
        vectorOut = vector3(randomDoubleRange(-1, 1), randomDoubleRange(-1, 1), 0);
        vectorLengthSquared = lengthSquared(vectorOut);
    } while (vectorLengthSquared >= 1.0);

    return vectorOut;
}


bool isNearlyZero(Vector3 v)
{
    return (fabs(v.x) < kZeroTolerance && fabs(v.y) < kZeroTolerance && fabs(v.z) < kZeroTolerance);
}


int randomInt(int min, int max)
{
    return (int)randomDoubleRange(min, max + 1);
}


Vector3 zeroVector(void)
{
    return vector3(0, 0, 0);
}
