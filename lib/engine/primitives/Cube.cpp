/**
 * @file Cube.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Cube.hpp"

extern "C"
{
#include "utility/MathMacros.h"
}


Cube::Cube(Point3 center_, Vector3 rotAngles_, double length_, std::shared_ptr<Material> material_)
    : Primitive(material_), center(center_), length(length_)
{
    rotationMatrix = makeRotate3(rotAngles_);
}

Cube::~Cube()
{
    if (rotationMatrix)
    {
        free(rotationMatrix);
    }
}


bool Cube::hit(Ray *ray, double tmin, double tmax, Hit *hit)
{
    const double halfLength = 0.5 * length;

    // Transform ray and shift it so that cube is at the origin and oriented
    // along the y-axis.
    Ray tranRay = transformRay(ray, center, rotationMatrix);
    Point3 tOrigin = tranRay.origin;
    Vector3 tDir = tranRay.direction;

    double tEnter, tExit;
    const double divX = 1.0 / tDir.x;

    if (divX >= 0)
    {
        tEnter = (-halfLength - tOrigin.x) * divX;
        tExit = (+halfLength - tOrigin.x) * divX;
    }
    else
    {
        tExit = (-halfLength - tOrigin.x) * divX;
        tEnter = (+halfLength - tOrigin.x) * divX;
    }

    double tyEnter, tyExit;
    const double divY = 1.0 / tDir.y;

    if (divY >= 0)
    {
        tyEnter = (-halfLength - tOrigin.y) * divY;
        tyExit = (+halfLength - tOrigin.y) * divY;
    }
    else
    {
        tyExit = (-halfLength - tOrigin.y) * divY;
        tyEnter = (+halfLength - tOrigin.y) * divY;
    }

    tEnter = max(tEnter, tyEnter);
    tExit = min(tExit, tyExit);

    if (tExit < tEnter) return false; // No intersection.

    double tzEnter, tzExit;
    const double divZ = 1.0 / tDir.z;

    if (divZ >= 0)
    {
        tzEnter = (-halfLength - tOrigin.z) * divZ;
        tzExit = (+halfLength - tOrigin.z) * divZ;
    }
    else
    {
        tzExit = (-halfLength - tOrigin.z) * divZ;
        tzEnter = (+halfLength - tOrigin.z) * divZ;
    }

    tEnter = max(tEnter, tzEnter);
    tExit = min(tExit, tzExit);

    if (tExit < tEnter || tEnter > tmax || tEnter < tmin) return false;

    const double hitTime = tEnter;
    Vector3 outwardNormal;

    // The hit will occur at time tmin. Now calculate which face it intersects.
    if (tyEnter == hitTime)
        outwardNormal = vector3(0, (divY >= 0 ? -1 : 1), 0);
    else if (tzEnter == hitTime)
        outwardNormal = vector3(0, 0, (divZ >= 0 ? -1 : 1));
    else
        outwardNormal = vector3((divX >= 0 ? -1 : 1), 0, 0);

    // Calculate the hit point and outward normal in original coordinates
    // (rotate back to original). hitTime is correct in both coordinates.
    Vector3 hitPoint = ray->pointAtTime(hitTime);
    outwardNormal = rotation(outwardNormal, rotationMatrix);

    const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

    hit->frontFace = frontFace;
    hit->t = hitTime;
    hit->hitPt = hitPoint;
    hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
    hit->material = material.get();

    hit->u = 0.0;
    hit->v = 0.0;

    return true;
}


bool Cube::boundingBox(AABB *outputBox)
{
    const double halfL = 0.5 * length;

    if (!rotationMatrix)
    {
        outputBox->minPt() = point3(center.x - halfL, center.y - halfL, center.z - halfL);
        outputBox->maxPt() = point3(center.x + halfL, center.y + halfL, center.z + halfL);
    }
    else
    {
        outputBox->reset();

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    Point3 vertex; // Vertex before rotation or translation.

                    vertex.x = (i == 0) ? -halfL : halfL;
                    vertex.y = (j == 0) ? -halfL : halfL;
                    vertex.z = (k == 0) ? -halfL : halfL;

                    // Rotated and then translated vertex.
                    Point3 vertexPrime = addVectors(rotation(vertex, rotationMatrix), center);

                    outputBox->addPoint(vertexPrime);
                }
            }
        }
    }

    return true;
}
