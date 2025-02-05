//
//  Primitive.h
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#ifndef Primitive_h
#define Primitive_h

#include "engine/HitRec.h"
#include "engine/Material.h"
#include "engine/Ray.h"
#include "models/Fractals3D.h"
#include "utility/Matrix3.h"
#include "utility/Vector3.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct bvhNode_t BVHNode;
typedef struct cube_t Cube;
typedef struct sphere_t Sphere;
typedef struct cylinder_t Cylinder;
typedef struct disc_t Disc;
typedef struct plane_t Plane;
typedef struct cone_t Cone;
typedef struct aabb_t AABB;
typedef struct primitive_t Primitive;
typedef struct triangle_t Triangle;
typedef struct fractal_t Fractal;

struct primitive_t
{
    union
    {
        BVHNode *node;
        Triangle *triangle;
        Cube *cube;
        Sphere *sphere;
        Cylinder *cylinder;
        Disc *disc;
        Plane *plane;
        Cone *cone;
        Fractal *fractal;
    };

    Material *material;

    bool (*hit)(Primitive *, Ray *, double, double, HitRec *);
    bool (*boundingBox)(Primitive *, AABB *);
    void (*destructor)(Primitive *);
};


struct aabb_t // Axis-aligned Bounding box.
{
    Point3 min, max;
};


Primitive *makeCube(Point3 center, Vector3 rotAngles, double length, Material *material);
Primitive *makeSphere(Point3 center, double radius, Material *material);
Primitive *makePlane(Point3 p0, Vector3 normal, Material *material);
Primitive *makeDisc(Point3 center, Vector3 normal, double radius, Material *material);
Primitive *makeCylinder(Point3 center, Vector3 rotAngles, double radius, double height, Material *material);
Primitive *makeCone(Point3 center, Vector3 rotAngles, double height, Material *material);
Primitive *makeBVHNode(Primitive **objects, int start, int end);
Primitive *makeTriangle(Point3 v0, Point3 v1, Point3 v2, Material *material);

#endif /* Primitive_h */
