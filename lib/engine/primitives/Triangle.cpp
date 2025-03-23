/**
 * @file Triangle.cpp
 * @author Edward Palmer
 * @date 2025-03-02
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Triangle.hpp"

Triangle::Triangle(Point3 v0_, Point3 v1_, Point3 v2_, std::shared_ptr<Material> material_)
    : Primitive(material_), v0(v0_), v1(v1_), v2(v2_)
{
    normal = cross(subtractVectors(v1, v0), subtractVectors(v2, v1));
}

bool Triangle::hit(Ray *ray, double tmin, double tmax, HitRec *hit)
{
    // Triangle can be defined in terms of coordinates (u, v):
    // T(u, v) = (1 - u - v) * V0 + u * V1 + v * V2
    // where V0, V1, V2 are the vertices, u >= 0, v >= 0 and u + 1 <= 1

    // For intersection between ray R(t) and triangle T(u, v):
    // R(t)   = T(u, v)
    // O + tD = (1 - u - v)V0 + uV1 + vV2
    //
    // Rearrange:
    //                            [ t ]
    // [ -D, V1 - V0, V2 - V0 ] * [ u ] = O - V0
    //							  [ v ]
    //
    // Let E1 = V1 - V0, E2 = V2 - V0, T = O - V0
    //
    // [ t ]                               [ det( [T, E1, E2 ] ) ]
    // [ u ] =  1 / det( [-D, E1, E2 ] ) * [ det( [-D, T, E2 ] ) ]
    // [ v ]                               [ det( [ -D, E1, T ] ) ]
    //
    // From applying Cramer's rule.
    // Also, | A, B, C | = -(A x C).B = -(C x B).A
    //
    // [ t ]                     [ (T x E1).E2 ]                [ Q.E2]
    // [ u ] = 1 / (D x E2).E1 * [ (D x E2).T  ] = 1 / (P.E1) * [ P.T ]
    // [ v ]                     [ (T x E1).D  ]                [ Q.D ]
    //
    // where P = D x E2, Q = T x E1

    double hitTime = 0.0;

    if (intersectionWithPlane(v0, normal, ray, &hitTime) && isValidIntersectionTime(hitTime, tmin, tmax))
    {
        Vector3 vecO = ray->origin;
        Vector3 vecD = ray->direction;

        Vector3 vecE1 = subtractVectors(v1, v0);
        Vector3 vecE2 = subtractVectors(v2, v0);
        Vector3 vecT = subtractVectors(vecO, v0);

        Vector3 vecP = cross(vecD, vecE2);

        const double invPDotE1 = 1.0 / dot(vecP, vecE1);

        const double u = invPDotE1 * dot(vecP, vecT);

        if (u < 0.0 || u > 1.0) return false;

        Vector3 vecQ = cross(vecT, vecE1);

        const double v = invPDotE1 * dot(vecQ, vecD);

        if (v < 0.0 || u + v > 1.0) return false;

        // Compute the normal vector:
        Point3 hitPoint = pointAtTime(ray, hitTime);
        Vector3 outwardNormal = normal;

        // Are we hitting the outside surface or are we hitting the inside?
        const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

        hit->frontFace = frontFace;
        hit->t = hitTime;
        hit->hitPt = hitPoint;
        hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
        hit->material = material.get();

        hit->u = u;
        hit->v = v;

        return true;
    }

    return false;
}


bool Triangle::boundingBox(AABB *outputBox)
{
    outputBox->reset();

    outputBox->addPoint(v0);
    outputBox->addPoint(v1);
    outputBox->addPoint(v2);

    return true;
}