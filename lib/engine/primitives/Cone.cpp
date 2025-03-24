/**
 * @file Cone.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Cone.hpp"

Cone::Cone(Point3 center_, Vector3 rotAngles_, double height_, std::shared_ptr<Material> material_)
    : Primitive(material_), center(center_), height(height_),
      base(point3(0, height, 0), vector3(0, 1, 0), height, material), rotationMatrix(makeRotate3(rotAngles_))
{
}


Cone::~Cone()
{
    if (rotationMatrix)
    {
        free(rotationMatrix);
    }
}


bool Cone::hit(Ray &ray, Time tmin, Time tmax, Hit &hit)
{
    Ray tranRay = transformRay(ray, center, rotationMatrix);
    Vector3 tOrigin = tranRay.origin;
    Vector3 tdir = tranRay.direction;

    const double quadA = (tdir.x * tdir.x + tdir.z * tdir.z - tdir.y * tdir.y);
    const double quadB = 2 * (tOrigin.x * tdir.x + tOrigin.z * tdir.z - tOrigin.y * tdir.y);
    const double quadC = (tOrigin.x * tOrigin.x + tOrigin.z * tOrigin.z - tOrigin.y * tOrigin.y);

    double t1, t2;

    if (!solveQuadratic(quadA, quadB, quadC, &t1, &t2)) return false;

    double hitTime = tmax; // Initialize to maximum time.
    Point3 hitPoint = {0};
    Vector3 outwardNormal = {0};

    // Check for intersection with cone side:
    const bool t1Valid = Hit::isValid(t1, tmin, tmax);
    const bool t2Valid = Hit::isValid(t2, tmin, tmax);

    double minY = 0, maxY = height;

    if (t1Valid || t2Valid)
    {
        Point3 hitPt1 = tranRay.pointAtTime(t1);
        Point3 hitPt2 = tranRay.pointAtTime(t2);

        const bool hitPt1Valid = (hitPt1.y > minY && hitPt1.y < maxY);
        const bool hitPt2Valid = (hitPt2.y > minY && hitPt2.y < maxY);

        if (t1Valid && hitPt1Valid)
        {
            hitTime = t1;
            outwardNormal = unitVector(vector3(hitPt1.x, -hitPt1.y, hitPt1.z));
        }
        else if (t2Valid && hitPt2Valid)
        {
            hitTime = t2;
            outwardNormal = unitVector(vector3(hitPt2.x, -hitPt2.y, hitPt2.z));
        }
    }

    // Check for a closer intersection from hitting the base of the cone:
    if (base.hit(tranRay, tmin, hitTime, hit))
    {
        hitTime = hit.t;
        outwardNormal = hit.normal;
    }

    // No intersections --> return false;
    if (hitTime == tmax) return false;

    // Calculate the hit point in original coordinates and rotate normal:
    hitPoint = ray.pointAtTime(hitTime);
    outwardNormal = rotation(outwardNormal, rotationMatrix);

    const bool frontFace = (dot(ray.direction, outwardNormal) < 0.0);

    hit.frontFace = frontFace;
    hit.t = hitTime;
    hit.hitPt = hitPoint;
    hit.normal = frontFace ? outwardNormal : flipVector(outwardNormal);
    hit.material = material.get();

    hit.u = 0.0;
    hit.v = 0.0;

    return true;
}


bool Cone::boundingBox(AABB *outputBox)
{
    if (!rotationMatrix)
    {
        outputBox->minPt() = point3(center.x - height, center.y, center.z - height);
        outputBox->maxPt() = point3(center.x + height, center.y + height, center.z + height);
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

                    vertex.x = (i == 0) ? -height : height;
                    vertex.y = (j == 0) ? 0.0 : height;
                    vertex.z = (k == 0) ? -height : height;

                    // Rotated and then translated vertex.
                    Point3 vertexPrime = addVectors(rotation(vertex, rotationMatrix), center);

                    outputBox->addPoint(vertexPrime);
                }
            }
        }
    }

    return true;
}
