/**
 * @file Cylinder.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Cylinder.hpp"

Cylinder::Cylinder(Point3 center_, Vector3 rotAngles_, double radius_, double height_,
                   std::shared_ptr<Material> material_)
    : Primitive(material_), center(center_), radius(radius_), height(height_),
      topCap(point3(0, height / 2.0, 0), vector3(0, 1, 0), radius, material),
      bottomCap(point3(0, -height / 2.0, 0), vector3(0, -1, 0), radius, material),
      rotationMatrix(makeRotate3(rotAngles_))
{
}

Cylinder::~Cylinder()
{
    if (rotationMatrix)
    {
        free(rotationMatrix);
    }
}


bool Cylinder::hit(Ray &ray, Time tmin, Time tmax, Hit &hit)
{
    const double ymin = -height / 2.0;
    const double ymax = height / 2.0;

    // Transform the ray by rotating and shifting it so that the cylinder is
    // centered at the origin. In this rotated space, the cylinder is oriented
    // along the y-axis.
    Ray tranRay = transformRay(ray, center, rotationMatrix);
    Point3 tOrigin = tranRay.origin;
    Vector3 tDir = tranRay.direction;

    // Solve:
    const double quadA = tDir.x * tDir.x + tDir.z * tDir.z;
    const double quadB = 2.0 * (tDir.x * tOrigin.x + tDir.z * tOrigin.z);
    const double quadC = (tOrigin.x * tOrigin.x + tOrigin.z * tOrigin.z) - (radius * radius);

    double t1, t2;

    if (!solveQuadratic(quadA, quadB, quadC, &t1, &t2)) return false;

    const bool t1Valid = Hit::isValid(t1, tmin, tmax);
    const bool t2Valid = Hit::isValid(t2, tmin, tmax);

    double hitTime = tmax; // Set to tmax initially.
    Point3 hitPoint = {0};
    Vector3 outwardNormal = {0};

    // Check for intersections with side of cylinder:
    if (t1Valid || t2Valid)
    {
        Point3 hitPt1 = tranRay.pointAtTime(t1);
        Point3 hitPt2 = tranRay.pointAtTime(t2);

        const bool hitPt1Valid = (hitPt1.y > ymin && hitPt1.y < ymax);
        const bool hitPt2Valid = (hitPt2.y > ymin && hitPt2.y < ymax);

        if (t1Valid && hitPt1Valid)
        {
            hitTime = t1;
            outwardNormal = unitVector(vector3(hitPt1.x, 0, hitPt1.z));
        }
        else if (t2Valid && hitPt2Valid)
        {
            hitTime = t2;
            outwardNormal = unitVector(vector3(hitPt2.x, 0, hitPt2.z));
        }
    }

    // Check for closer intersection with bottom or top cap:
    if (topCap.hit(tranRay, tmin, hitTime, hit))
    {
        hitTime = hit.t;
        outwardNormal = hit.normal;
    }
    else if (bottomCap.hit(tranRay, tmin, hitTime, hit))
    {
        hitTime = hit.t;
        outwardNormal = hit.normal;
    }

    // Check for any hits:
    if (hitTime == tmax) return false;

    // Calculate the hit point and outward normal in original coordinates
    // (rotate back to original). hitTime is correct in both coordinates.
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


bool Cylinder::boundingBox(AABB *outputBox)
{
    const double halfHeight = 0.5 * height;

    if (!rotationMatrix)
    {
        outputBox->minPt() = point3(center.x - radius, center.y - halfHeight, center.z - radius);
        outputBox->maxPt() = point3(center.x + radius, center.y + halfHeight, center.z + radius);
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

                    vertex.x = (i == 0) ? -radius : radius;
                    vertex.y = (j == 0) ? -halfHeight : halfHeight;
                    vertex.z = (k == 0) ? -radius : radius;

                    // Rotated and then translated vertex.
                    Point3 vertexPrime = addVectors(rotation(vertex, rotationMatrix), center);

                    outputBox->addPoint(vertexPrime);
                }
            }
        }
    }

    return true;
}
