//
//  Sphere.c
//  CPhoton
//
//  Created by Edward on 19/01/2023.
//

#include "engine/Primitive.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Function Prototypes:

#define swap(val1, val2) ({ double temp = (val1); (val1) = (val2); (val2) = temp; })

static inline bool isValidIntersectionTime(double hitTime, double tmin, double tmax);
static inline bool intersectionWithPlane(Point3 p0, Vector3 n, Ray *ray, double *hitTime);
static inline bool isSphereHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit);
static inline bool isCylinderHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit);
static inline bool isPlaneHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit);
static inline bool isDiscHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit);
static inline bool isConeHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit);
static inline bool isCubeHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit);
static inline bool isBoundingBoxHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit);
static inline bool isTriangleHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit);

static inline bool solveQuadratic(double a, double b, double c, double *t1, double *t2);
static inline Ray transformRay(Ray *ray, Point3 center, Rotate3 *rotation);

static inline void resetBoundingBox(AABB *outputBox);
static inline void addPointToBoundingBox(Point3 pt, AABB *outputBox);

static inline bool sphereBoundingBox(Primitive *primitive, AABB *outputBox);
static inline bool cylinderBoundingBox(Primitive *primitive, AABB *outputBox);
static inline bool planeBoundingBox(Primitive *primive, AABB *outputBox);
static inline bool discBoundingBox(Primitive *primive, AABB *outputBox);
static inline bool coneBoundingBox(Primitive *primive, AABB *outputBox);
static inline bool cubeBoundingBox(Primitive *primive, AABB *outputBox);
static inline bool BVHNodeBoundingBox(Primitive *primitive, AABB *outputBox);
static inline bool triangleBoundingBox(Primitive *primitive, AABB *outputBox);

static inline AABB addBoundingBoxes(AABB box0, AABB box1);
static inline int compareBoundingBoxes(AABB *boxA, AABB *boxB, int axis);
static inline bool isAABBHit(AABB *aabb, Ray *ray, double tmin, double tmax);

static void destructCube(Primitive *primitive);
static void destructSphere(Primitive *primitive);
static void destructCylinder(Primitive *primitive);
static void destructPlane(Primitive *primitive);
static void destructDisc(Primitive *primitive);
static void destructCone(Primitive *primitive);
static void destructBVHNode(Primitive *primitive);
static void destructTriangle(Primitive *primitive);

static inline void setSphereUV(Vector3 *outwardNormal, double *u, double *v);


// *** Primitives ***

struct cone_t
{
    Point3 center;
    double height;
    Rotate3 *rotation;
    Primitive *base;
};


struct bvhNode_t
{
    AABB box;
    Primitive *left;
    Primitive *right;
};


struct sphere_t
{
    Point3 center;
    double radius;
};


struct cylinder_t
{
    Point3 center;
    double height;
    double radius;
    Rotate3 *rotation;

    Primitive *topCap;
    Primitive *bottomCap;
};


struct plane_t
{
    Point3 p0; // Point on the plane.
    Point3 normal;
};


struct disc_t
{
    Plane plane;
    double radius;
};


struct cube_t
{
    Point3 center;
    double length;
    Rotate3 *rotation;
};


struct triangle_t
{
    Point3 v0, v1, v2;
    Vector3 normal; // TODO: - later, each vertex will have its own normal.
};


// *** Axis-Aligned Bounded Boxes ***

static inline void resetBoundingBox(AABB *outputBox)
{
    if (!outputBox)
        return;

    outputBox->min = point3(INFINITY, INFINITY, INFINITY);
    outputBox->max = point3(-INFINITY, -INFINITY, -INFINITY);
}


static inline void addPointToBoundingBox(Point3 pt, AABB *outputBox)
{
    if (pt.x < outputBox->min.x)
        outputBox->min.x = pt.x;
    if (pt.x > outputBox->max.x)
        outputBox->max.x = pt.x;

    if (pt.y < outputBox->min.y)
        outputBox->min.y = pt.y;
    if (pt.y > outputBox->max.y)
        outputBox->max.y = pt.y;

    if (pt.z < outputBox->min.z)
        outputBox->min.z = pt.z;
    if (pt.z > outputBox->max.z)
        outputBox->max.z = pt.z;
}


static inline bool sphereBoundingBox(Primitive *primitive, AABB *outputBox)
{
    Point3 center = primitive->sphere->center;
    const double r = primitive->sphere->radius;

    outputBox->min = point3(center.x - r, center.y - r, center.z - r);
    outputBox->max = point3(center.x + r, center.y + r, center.z + r);

    return true;
}


static inline bool cubeBoundingBox(Primitive *primitive, AABB *outputBox)
{
    Cube *cube = primitive->cube;

    Point3 center = cube->center;
    const double halfL = 0.5 * cube->length;

    if (!cube->rotation)
    {
        outputBox->min = point3(center.x - halfL, center.y - halfL, center.z - halfL);
        outputBox->max = point3(center.x + halfL, center.y + halfL, center.z + halfL);
    }
    else
    {
        resetBoundingBox(outputBox);

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
                    Point3 vertexPrime = addVectors(rotation(vertex, cube->rotation), center);

                    addPointToBoundingBox(vertexPrime, outputBox);
                }
            }
        }
    }

    return true;
}


static inline bool BVHNodeBoundingBox(Primitive *primitive, AABB *outputBox)
{
    *outputBox = primitive->node->box;
    return true;
}


static inline bool cylinderBoundingBox(Primitive *primitive, AABB *outputBox)
{
    Cylinder *cylinder = primitive->cylinder;
    Point3 center = cylinder->center;

    const double halfHeight = 0.5 * cylinder->height;
    const double radius = cylinder->radius;

    if (!cylinder->rotation)
    {
        outputBox->min = point3(center.x - radius, center.y - halfHeight, center.z - radius);
        outputBox->max = point3(center.x + radius, center.y + halfHeight, center.z + radius);
    }
    else
    {
        resetBoundingBox(outputBox);

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
                    Point3 vertexPrime = addVectors(rotation(vertex, cylinder->rotation), center);

                    addPointToBoundingBox(vertexPrime, outputBox);
                }
            }
        }
    }

    return true;
}


static inline bool planeBoundingBox(Primitive *primitive, AABB *outputBox)
{
    Plane *plane = primitive->plane;
    Point3 center = plane->p0;
    Vector3 normal = plane->normal;
    const double deltaR = 0.001;

    if (fabs(normal.x) == 1.0)
    {
        outputBox->min = point3(center.x - deltaR, -INFINITY, -INFINITY);
        outputBox->max = point3(center.x + deltaR, INFINITY, INFINITY);
    }
    else if (fabs(normal.y) == 1.0)
    {
        outputBox->min = point3(-INFINITY, center.y - deltaR, -INFINITY);
        outputBox->max = point3(INFINITY, center.y + deltaR, INFINITY);
    }
    else if (fabs(normal.z) == 1.0)
    {
        outputBox->min = point3(-INFINITY, -INFINITY, center.z - deltaR);
        outputBox->max = point3(INFINITY, INFINITY, center.z + deltaR);
    }
    else
    {
        outputBox->min = point3(-INFINITY, -INFINITY, -INFINITY);
        outputBox->max = point3(INFINITY, INFINITY, INFINITY);
    }

    return true;
}


static inline bool discBoundingBox(Primitive *primitive, AABB *outputBox)
{
    Disc *disc = primitive->disc;
    Point3 center = disc->plane.p0;
    Vector3 normal = disc->plane.normal;

    const double radius = disc->radius;
    const double deltaR = 0.001;

    if (fabs(normal.x) == 1.0)
    {
        outputBox->min = point3(center.x - deltaR, center.y - radius, center.z - radius);
        outputBox->max = point3(center.x + deltaR, center.y + radius, center.z + radius);
    }
    else if (fabs(normal.y) == 1.0)
    {
        outputBox->min = point3(center.x - radius, center.y - deltaR, center.z - radius);
        outputBox->max = point3(center.x + radius, center.y + deltaR, center.z + radius);
    }
    else if (fabs(normal.z) == 1.0)
    {
        outputBox->min = point3(center.x - radius, center.y - radius, center.z - deltaR);
        outputBox->max = point3(center.x + radius, center.y + radius, center.z + deltaR);
    }
    else
    {
        outputBox->min = point3(center.x - radius, center.y - radius, center.z - radius);
        outputBox->max = point3(center.x + radius, center.y + radius, center.z + radius);
    }

    return true;
}


static inline bool coneBoundingBox(Primitive *primitive, AABB *outputBox)
{
    Cone *cone = primitive->cone;
    Point3 center = cone->center;

    const double height = cone->height;

    if (!cone->rotation)
    {
        outputBox->min = point3(center.x - height, center.y, center.z - height);
        outputBox->max = point3(center.x + height, center.y + height, center.z + height);
    }
    else
    {
        resetBoundingBox(outputBox);

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
                    Point3 vertexPrime = addVectors(rotation(vertex, cone->rotation), center);

                    addPointToBoundingBox(vertexPrime, outputBox);
                }
            }
        }
    }

    return true;
}


static inline bool triangleBoundingBox(Primitive *primitive, AABB *outputBox)
{
    Triangle *triangle = primitive->triangle;

    resetBoundingBox(outputBox);

    addPointToBoundingBox(triangle->v0, outputBox);
    addPointToBoundingBox(triangle->v1, outputBox);
    addPointToBoundingBox(triangle->v2, outputBox);

    return true;
}


// *** AABB Functions ***

static inline AABB addBoundingBoxes(AABB box0, AABB box1)
{
    Point3 newMin = point3(min(box0.min.x, box1.min.x), min(box0.min.y, box1.min.y), min(box0.min.z, box1.min.z));
    Point3 newMax = point3(max(box0.max.x, box1.max.x), max(box0.max.y, box1.max.y), max(box0.max.z, box1.max.z));

    AABB newBoundingBox = {.min = newMin, .max = newMax};

    return newBoundingBox;
}


static inline int compareBoundingBoxes(AABB *boxA, AABB *boxB, int axis)
{
    double minA, minB;

    if (axis == 0)
    {
        minA = boxA->min.x;
        minB = boxB->min.x;
    }
    else if (axis == 1)
    {
        minA = boxA->min.y;
        minB = boxB->min.y;
    }
    else
    {
        minA = boxA->min.z;
        minB = boxB->min.z;
    }

    if (minA < minB)
        return (-1);
    else if (minA > minB)
        return 1;
    else
        return 0;
}


int boxComparatorX(const void *ptr1, const void *ptr2)
{
    Primitive *objectA = *(Primitive **)ptr1;
    Primitive *objectB = *(Primitive **)ptr2;

    AABB boxA, boxB;

    if (!objectA->boundingBox(objectA, &boxA) || !objectB->boundingBox(objectB, &boxB))
    {
        return 0;
    }

    return compareBoundingBoxes(&boxA, &boxB, 0);
}


int boxComparatorY(const void *ptr1, const void *ptr2)
{
    Primitive *objectA = *(Primitive **)ptr1;
    Primitive *objectB = *(Primitive **)ptr2;

    AABB boxA, boxB;

    if (!objectA->boundingBox(objectA, &boxA) || objectB->boundingBox(objectB, &boxB))
    {
        return 0;
    }

    return compareBoundingBoxes(&boxA, &boxB, 1);
}


int boxComparatorZ(const void *ptr1, const void *ptr2)
{
    Primitive *objectA = *(Primitive **)ptr1;
    Primitive *objectB = *(Primitive **)ptr2;

    AABB boxA, boxB;

    if (!objectA->boundingBox(objectA, &boxA) || !objectB->boundingBox(objectB, &boxB))
    {
        return 0;
    }

    return compareBoundingBoxes(&boxA, &boxB, 2);
}


// *** Destructors ***

static void destructTriangle(Primitive *primitive)
{
    if (primitive)
    {
        if (primitive->triangle)
            free(primitive->triangle);

        primitive->material->decrementRefCounter(primitive->material);
        free(primitive);
    }
}


static void destructCube(Primitive *primitive)
{
    if (primitive)
    {
        Cube *cube = primitive->cube;
        if (cube)
        {
            if (cube->rotation)
                free(cube->rotation);

            free(cube);
        }

        primitive->material->decrementRefCounter(primitive->material);
        free(primitive);
    }
}


static void destructSphere(Primitive *primitive)
{
    if (primitive)
    {
        Sphere *sphere = primitive->sphere;
        if (sphere)
            free(sphere);

        primitive->material->decrementRefCounter(primitive->material);
        free(primitive);
    }
}


static void destructCylinder(Primitive *primitive)
{
    if (primitive)
    {
        Cylinder *cylinder = primitive->cylinder;
        if (cylinder)
        {
            if (cylinder->rotation)
                free(cylinder->rotation);

            if (cylinder->topCap)
                cylinder->topCap->destructor(cylinder->topCap);
            if (cylinder->bottomCap)
                cylinder->bottomCap->destructor(cylinder->bottomCap);

            free(cylinder);
        }

        primitive->material->decrementRefCounter(primitive->material);
        free(primitive);
    }
}


static void destructPlane(Primitive *primitive)
{
    if (primitive)
    {
        Plane *plane = primitive->plane;
        if (plane)
            free(plane);

        primitive->material->decrementRefCounter(primitive->material);
        free(primitive);
    }
}


static void destructDisc(Primitive *primitive)
{
    if (primitive)
    {
        Disc *disc = primitive->disc;
        if (disc)
            free(disc);

        primitive->material->decrementRefCounter(primitive->material);
        free(primitive);
    }
}


static void destructCone(Primitive *primitive)
{
    if (primitive)
    {
        Cone *cone = primitive->cone;
        if (cone)
        {
            if (cone->rotation)
                free(cone->rotation);
            if (cone->base)
                cone->base->destructor(cone->base);

            free(cone);
        }

        primitive->material->decrementRefCounter(primitive->material);
        free(primitive);
    }
}


static void destructBVHNode(Primitive *primitive)
{
    if (primitive)
    {
        BVHNode *node = primitive->node;
        if (node)
        {
            if (node->left)
                node->left->destructor(node->left);
            if (node->right)
                node->right->destructor(node->right);

            free(node);
        }

        free(primitive);
    }
}


// *** Make Primitives ***

Primitive *makeCube(Point3 center, Vector3 rotAngles, double length, Material *material)
{
    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive)
        return NULL;

    Cube *cube = malloc(sizeof(Cube));
    if (!cube)
    {
        free(primitive);
        return NULL;
    }

    cube->length = length;
    cube->center = center;
    cube->rotation = makeRotate3(rotAngles);

    primitive->cube = cube;
    primitive->hit = isCubeHit;
    primitive->boundingBox = cubeBoundingBox;
    primitive->destructor = destructCube;
    primitive->material = material;

    material->incrementRefCounter(material);

    return primitive;
}


Primitive *makeSphere(Point3 center, double radius, Material *material)
{
    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive)
        return NULL;

    Sphere *sphere = malloc(sizeof(Sphere));
    if (!sphere)
    {
        free(primitive);
        return NULL;
    }

    sphere->center = center;
    sphere->radius = radius;

    primitive->sphere = sphere;
    primitive->hit = isSphereHit;
    primitive->boundingBox = sphereBoundingBox;
    primitive->destructor = destructSphere;
    primitive->material = material;

    material->incrementRefCounter(material);

    return primitive;
}


Primitive *makeCylinder(Point3 center, Vector3 rotAngles, double radius, double height, Material *material)
{
    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive)
        return NULL;

    Cylinder *cylinder = malloc(sizeof(Cylinder));
    if (!cylinder)
    {
        free(primitive);
        return NULL;
    }

    Primitive *topCap = makeDisc(point3(0, height / 2.0, 0), vector3(0, 1, 0), radius, material);
    if (!topCap)
    {
        free(primitive);
        free(cylinder);
        return NULL;
    }

    Primitive *bottomCap = makeDisc(point3(0, -height / 2.0, 0), vector3(0, -1, 0), radius, material);
    if (!bottomCap)
    {
        free(primitive);
        free(cylinder);
        free(topCap);
        return NULL;
    }

    cylinder->center = center;
    cylinder->radius = radius;
    cylinder->height = height;
    cylinder->topCap = topCap;
    cylinder->bottomCap = bottomCap;
    cylinder->rotation = makeRotate3(rotAngles);

    primitive->cylinder = cylinder;
    primitive->hit = isCylinderHit;
    primitive->boundingBox = cylinderBoundingBox;
    primitive->destructor = destructCylinder;
    primitive->material = material;

    material->incrementRefCounter(material);

    return primitive;
}


Primitive *makePlane(Point3 p0, Vector3 normal, Material *material)
{
    normal = isNearlyZero(normal) ? vector3(0, 1, 0) : unitVector(normal);

    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive)
        return NULL;

    Plane *plane = malloc(sizeof(Plane));
    if (!plane)
    {
        free(primitive);
        return NULL;
    }

    plane->p0 = p0;
    plane->normal = normal;

    primitive->plane = plane;
    primitive->hit = isPlaneHit;
    primitive->boundingBox = planeBoundingBox;
    primitive->destructor = destructPlane;
    primitive->material = material;

    material->incrementRefCounter(material);

    return primitive;
}


Primitive *makeDisc(Point3 center, Point3 normal, double radius, Material *material)
{
    normal = isNearlyZero(normal) ? vector3(0, 1, 0) : unitVector(normal);

    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive)
        return NULL;

    Disc *disc = malloc(sizeof(Disc));
    if (!disc)
    {
        free(primitive);
        return NULL;
    }

    disc->radius = radius;
    disc->plane.p0 = center;
    disc->plane.normal = normal;

    primitive->disc = disc;
    primitive->hit = isDiscHit;
    primitive->boundingBox = discBoundingBox;
    primitive->destructor = destructDisc;
    primitive->material = material;

    material->incrementRefCounter(material);

    return primitive;
}


Primitive *makeCone(Point3 center, Vector3 rotAngles, double height, Material *material)
{
    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive)
        return NULL;

    Cone *cone = malloc(sizeof(Cone));
    if (!cone)
    {
        free(primitive);
        return NULL;
    }

    Point3 baseCenter = point3(0, height, 0);
    Primitive *coneBase = makeDisc(baseCenter, vector3(0, 1, 0), height, material);
    if (!coneBase)
    {
        free(primitive);
        free(cone);
        return NULL;
    }

    cone->base = coneBase;
    cone->center = center;
    cone->height = height;
    cone->rotation = makeRotate3(rotAngles);

    primitive->cone = cone;
    primitive->hit = isConeHit;
    primitive->boundingBox = coneBoundingBox;
    primitive->destructor = destructCone;
    primitive->material = material;

    material->incrementRefCounter(material);

    return primitive;
}


Primitive *makeTriangle(Point3 v0, Point3 v1, Point3 v2, Material *material)
{
    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive)
        return NULL;

    Triangle *triangle = malloc(sizeof(Triangle));
    if (!triangle)
    {
        free(primitive);
        return NULL;
    }

    triangle->v0 = v0;
    triangle->v1 = v1;
    triangle->v2 = v2;
    triangle->normal = cross(subtractVectors(v1, v0), subtractVectors(v2, v1));

    primitive->triangle = triangle;
    primitive->hit = isTriangleHit;
    primitive->boundingBox = triangleBoundingBox;
    primitive->destructor = destructTriangle;
    primitive->material = material;

    material->incrementRefCounter(material);

    return primitive;
}


Primitive *makeBVHNode(Primitive **objects, int start, int end)
{
    // Allocate memory and initialize primitive:
    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive)
        return NULL;

    BVHNode *node = malloc(sizeof(BVHNode));
    if (!node)
    {
        free(primitive);
        return NULL;
    };

    primitive->node = node;
    primitive->material = NULL;
    primitive->hit = isBoundingBoxHit;
    primitive->boundingBox = BVHNodeBoundingBox;
    primitive->destructor = destructBVHNode;

    const int objectSpan = (end - start);
    const int axis = randomInt(0, 2); // TODO: - split about largest axis.

    enum
    {
        kAxisX = 0,
        kAxisY = 1,
        kAxisZ = 2
    };

    int (*comparator)(const void *ptr1, const void *ptr2);

    comparator = (axis == kAxisX ? boxComparatorX : (axis == kAxisY ? boxComparatorY : boxComparatorZ));

    if (objectSpan == 1)
    {
        node->left = objects[start];
        node->right = NULL;

        node->left->boundingBox(node->left, &node->box); // Special case!
        return primitive;
    }
    else if (objectSpan == 2)
    {
        if (comparator(&objects[start], &objects[start + 1]) <= 0)
        {
            node->left = objects[start];
            node->right = objects[start + 1];
        }
        else
        {
            node->left = objects[start + 1];
            node->right = objects[start];
        }
    }
    else
    {
        qsort(objects + start, objectSpan, sizeof(Primitive *), comparator);

        int mid = start + objectSpan / 2;

        node->left = makeBVHNode(objects, start, mid);
        node->right = makeBVHNode(objects, mid, end);
    }

    // Calculate the node's bounding box (for objectSpan >= 2):
    AABB boxLeft, boxRight;

    if (!node->left->boundingBox(node->left, &boxLeft) || !node->right->boundingBox(node->right, &boxRight))
    {
        free(primitive);
        free(node);
        return NULL;
    }

    node->box = addBoundingBoxes(boxLeft, boxRight);
    return primitive;
}


// *** isHit Functions ***

static inline bool isTriangleHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit)
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

    Triangle *triangle = primitive->triangle;

    double hitTime = 0.0;

    if (intersectionWithPlane(triangle->v0, triangle->normal, ray, &hitTime) &&
        isValidIntersectionTime(hitTime, tmin, tmax))
    {
        Vector3 vecO = ray->origin;
        Vector3 vecD = ray->direction;

        Vector3 v0 = triangle->v0;
        Vector3 v1 = triangle->v1;
        Vector3 v2 = triangle->v2;

        Vector3 vecE1 = subtractVectors(v1, v0);
        Vector3 vecE2 = subtractVectors(v2, v0);
        Vector3 vecT = subtractVectors(vecO, v0);

        Vector3 vecP = cross(vecD, vecE2);

        const double invPDotE1 = 1.0 / dot(vecP, vecE1);

        const double u = invPDotE1 * dot(vecP, vecT);

        if (u < 0.0 || u > 1.0)
            return false;

        Vector3 vecQ = cross(vecT, vecE1);

        const double v = invPDotE1 * dot(vecQ, vecD);

        if (v < 0.0 || u + v > 1.0)
            return false;

        // Compute the normal vector:
        Point3 hitPoint = pointAtTime(ray, hitTime);
        Vector3 outwardNormal = triangle->normal;

        // Are we hitting the outside surface or are we hitting the inside?
        const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

        hit->frontFace = frontFace;
        hit->t = hitTime;
        hit->hitPt = hitPoint;
        hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
        hit->material = primitive->material;

        hit->u = u;
        hit->v = v;

        return true;
    }

    return false;
}


static inline bool isSphereHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit)
{
    Sphere *sphere = primitive->sphere;
    const double r = sphere->radius;

    // ray origin 	 = O
    // ray direction = d
    // sphere center = C
    // sphere radius = r
    //
    // ray = O + t * d
    //
    // Solve:
    // (ray - C)^2 = r^2
    //
    // t^2*(d.d) + 2t*d.(O - C) + (O - C).(O - C) - r^2 = 0
    //
    // Quadratic formula:
    // t = [-B +/- sqrt(B^2 - 4 * AC)] / 2A
    //
    // A = d.d
    // B = 2d.(O - C)
    // C = (O - C).(O - C) - r^2

    Vector3 rayOriginMinusCenter = subtractVectors(ray->origin, sphere->center);

    const double quadA = dot(ray->direction, ray->direction);
    const double quadB = 2.0 * dot(ray->direction, rayOriginMinusCenter);
    const double quadC = dot(rayOriginMinusCenter, rayOriginMinusCenter) - r * r;

    double t1, t2;

    if (!solveQuadratic(quadA, quadB, quadC, &t1, &t2))
        return false;

    double hitTime;

    if (isValidIntersectionTime(t1, tmin, tmax))
        hitTime = t1;
    else if (isValidIntersectionTime(t2, tmin, tmax))
        hitTime = t2;
    else
        return false;

    Point3 hitPoint = pointAtTime(ray, hitTime);

    // Compute the normal vector:
    Vector3 outwardNormal = scaleVector(subtractVectors(hitPoint, sphere->center), 1.0 / r);

    // Are we hitting the outside surface or are we hitting the inside?
    const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

    hit->frontFace = frontFace;
    hit->t = hitTime;
    hit->hitPt = hitPoint;
    hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
    hit->material = primitive->material;

    // Calculate the U, V texture coordinates:
    setSphereUV(&hit->normal, &hit->u, &hit->v);
    return true;
}


static inline bool isPlaneHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit)
{
    Plane *plane = primitive->plane;

    double hitTime = 0.0;

    if (intersectionWithPlane(plane->p0, plane->normal, ray, &hitTime) &&
        isValidIntersectionTime(hitTime, tmin, tmax))
    {
        Vector3 outwardNormal = plane->normal;

        const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

        hit->frontFace = frontFace;
        hit->t = hitTime;
        hit->hitPt = pointAtTime(ray, hitTime);
        hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
        hit->material = primitive->material;

        hit->u = 0.0;
        hit->v = 0.0;

        return true;
    }

    return false;
}


static inline bool isDiscHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit)
{
    Disc *disc = primitive->disc;

    double hitTime = 0.0;

    if (intersectionWithPlane(disc->plane.p0, disc->plane.normal, ray, &hitTime) &&
        isValidIntersectionTime(hitTime, tmin, tmax))
    {
        // Check that hit point is inside disc radius:
        Point3 hitPoint = pointAtTime(ray, hitTime);

        Vector3 hitPointMinusCenter = subtractVectors(hitPoint, disc->plane.p0);

        if (dot(hitPointMinusCenter, hitPointMinusCenter) > (disc->radius * disc->radius))
        {
            return false;
        }

        Vector3 outwardNormal = disc->plane.normal;

        const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

        hit->frontFace = frontFace;
        hit->t = hitTime;
        hit->hitPt = hitPoint;
        hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
        hit->material = primitive->material;

        hit->u = 0.0;
        hit->v = 0.0;

        return true;
    }

    return false;
}


static inline bool isAABBHit(AABB *aabb, Ray *ray, double tmin, double tmax)
{
    Vector3 minAABB = aabb->min;
    Vector3 maxAABB = aabb->max;
    Point3 origin = ray->origin;

    // Now test against x-direction.
    double invD = 1.0 / ray->direction.x;
    double t0 = (minAABB.x - origin.x) * invD;
    double t1 = (maxAABB.x - origin.x) * invD;

    if (invD < 0.0)
        swap(t0, t1);

    tmin = max(t0, tmin);
    tmax = min(t1, tmax);

    if (tmax <= tmin)
        return false;

    // Now test against y-direction.
    invD = 1.0 / ray->direction.y;
    t0 = (minAABB.y - origin.y) * invD;
    t1 = (maxAABB.y - origin.y) * invD;

    if (invD < 0.0)
        swap(t0, t1);

    tmin = max(t0, tmin);
    tmax = min(t1, tmax);

    if (tmax <= tmin)
        return false;

    // Now test against z-direction:
    invD = 1.0 / ray->direction.z;
    t0 = (minAABB.z - origin.z) * invD;
    t1 = (maxAABB.z - origin.z) * invD;

    if (invD < 0.0)
        swap(t0, t1);

    tmin = max(t0, tmin);
    tmax = min(t1, tmax);

    if (tmax <= tmin)
        return false;

    return true;
}


static inline bool isBoundingBoxHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit)
{
    BVHNode *node = primitive->node;

    if (!isAABBHit(&node->box, ray, tmin, tmax))
        return false;

    Primitive *left = node->left;
    Primitive *right = node->right;

    bool hitLeft = left && left->hit(left, ray, tmin, tmax, hit);
    bool hitRight = right && right->hit(right, ray, tmin, (hitLeft ? hit->t : tmax), hit);

    return (hitLeft || hitRight);
}


static inline bool isCubeHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit)
{
    Cube *cube = primitive->cube;
    const double halfLength = 0.5 * cube->length;

    // Transform ray and shift it so that cube is at the origin and oriented
    // along the y-axis.
    Ray tranRay = transformRay(ray, cube->center, cube->rotation);
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

    if (tExit < tEnter)
        return false; // No intersection.

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

    if (tExit < tEnter || tEnter > tmax || tEnter < tmin)
        return false;

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
    Vector3 hitPoint = pointAtTime(ray, hitTime);
    outwardNormal = rotation(outwardNormal, cube->rotation);

    const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

    hit->frontFace = frontFace;
    hit->t = hitTime;
    hit->hitPt = hitPoint;
    hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
    hit->material = primitive->material;

    hit->u = 0.0;
    hit->v = 0.0;

    return true;
}


static inline bool isCylinderHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit)
{
    Cylinder *cylinder = primitive->cylinder;

    const double radius = cylinder->radius;
    const double ymin = -cylinder->height / 2.0;
    const double ymax = cylinder->height / 2.0;

    // Transform the ray by rotating and shifting it so that the cylinder is
    // centered at the origin. In this rotated space, the cylinder is oriented
    // along the y-axis.
    Ray tranRay = transformRay(ray, cylinder->center, cylinder->rotation);
    Point3 tOrigin = tranRay.origin;
    Vector3 tDir = tranRay.direction;

    // Solve:
    const double quadA = tDir.x * tDir.x + tDir.z * tDir.z;
    const double quadB = 2.0 * (tDir.x * tOrigin.x + tDir.z * tOrigin.z);
    const double quadC = (tOrigin.x * tOrigin.x + tOrigin.z * tOrigin.z) - (radius * radius);

    double t1, t2;

    if (!solveQuadratic(quadA, quadB, quadC, &t1, &t2))
        return false;

    const bool t1Valid = isValidIntersectionTime(t1, tmin, tmax);
    const bool t2Valid = isValidIntersectionTime(t2, tmin, tmax);

    double hitTime = tmax; // Set to tmax initially.
    Point3 hitPoint = {0};
    Vector3 outwardNormal = {0};

    // Check for intersections with side of cylinder:
    if (t1Valid || t2Valid)
    {
        Point3 hitPt1 = pointAtTime(&tranRay, t1);
        Point3 hitPt2 = pointAtTime(&tranRay, t2);

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
    if (isDiscHit(cylinder->topCap, &tranRay, tmin, hitTime, hit))
    {
        hitTime = hit->t;
        outwardNormal = hit->normal;
    }
    else if (isDiscHit(cylinder->bottomCap, &tranRay, tmin, hitTime, hit))
    {
        hitTime = hit->t;
        outwardNormal = hit->normal;
    }

    // Check for any hits:
    if (hitTime == tmax)
        return false;

    // Calculate the hit point and outward normal in original coordinates
    // (rotate back to original). hitTime is correct in both coordinates.
    hitPoint = pointAtTime(ray, hitTime);
    outwardNormal = rotation(outwardNormal, cylinder->rotation);

    const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

    hit->frontFace = frontFace;
    hit->t = hitTime;
    hit->hitPt = hitPoint;
    hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
    hit->material = primitive->material;

    hit->u = 0.0;
    hit->v = 0.0;

    return true;
}


static inline bool isConeHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit)
{
    Cone *cone = primitive->cone;

    Ray tranRay = transformRay(ray, cone->center, cone->rotation);
    Vector3 tOrigin = tranRay.origin;
    Vector3 tdir = tranRay.direction;

    const double quadA = (tdir.x * tdir.x + tdir.z * tdir.z - tdir.y * tdir.y);
    const double quadB = 2 * (tOrigin.x * tdir.x + tOrigin.z * tdir.z - tOrigin.y * tdir.y);
    const double quadC = (tOrigin.x * tOrigin.x + tOrigin.z * tOrigin.z - tOrigin.y * tOrigin.y);

    double t1, t2;

    if (!solveQuadratic(quadA, quadB, quadC, &t1, &t2))
        return false;

    double hitTime = tmax; // Initialize to maximum time.
    Point3 hitPoint = {0};
    Vector3 outwardNormal = {0};

    // Check for intersection with cone side:
    const bool t1Valid = isValidIntersectionTime(t1, tmin, tmax);
    const bool t2Valid = isValidIntersectionTime(t2, tmin, tmax);

    double minY = 0, maxY = cone->height;

    if (t1Valid || t2Valid)
    {
        Point3 hitPt1 = pointAtTime(&tranRay, t1);
        Point3 hitPt2 = pointAtTime(&tranRay, t2);

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
    if (isDiscHit(cone->base, &tranRay, tmin, hitTime, hit))
    {
        hitTime = hit->t;
        outwardNormal = hit->normal;
    }

    // No intersections --> return false;
    if (hitTime == tmax)
        return false;

    // Calculate the hit point in original coordinates and rotate normal:
    hitPoint = pointAtTime(ray, hitTime);
    outwardNormal = rotation(outwardNormal, cone->rotation);

    const bool frontFace = (dot(ray->direction, outwardNormal) < 0.0);

    hit->frontFace = frontFace;
    hit->t = hitTime;
    hit->hitPt = hitPoint;
    hit->normal = frontFace ? outwardNormal : flipVector(outwardNormal);
    hit->material = primitive->material;

    hit->u = 0.0;
    hit->v = 0.0;

    return true;
}


// *** Texturing ***

/// Calculate the texture coordinates which are in range [0, 1] using the outward
/// normal calculated from the hit. We are using spherical polar coordinates with
/// theta being the angle from the +y axis and phi being the anticlockwise angle
/// starting from the +x axis.
static inline void setSphereUV(Vector3 *outwardNormal, double *u, double *v)
{
    const double theta = acos(-outwardNormal->y);
    const double phi = atan2(-outwardNormal->z, outwardNormal->x) + M_PI;

    *u = phi / (2.0 * M_PI);
    *v = theta / M_PI;
}


// *** Utility ***

static inline bool isValidIntersectionTime(double hitTime, double tmin, double tmax)
{
    return (tmin < hitTime && hitTime < tmax);
}


/// Returns a positive value > 0 if a ray intersects with a plane. p0 is an
/// arbitrary point on the plane and n is the plane's normal vector.
static inline bool intersectionWithPlane(Point3 p0, Vector3 n, Ray *ray, double *hitTime)
{
    // Eqn for line: p = origin + t * d
    // Plane: (p - p0).n = 0 where n is normal to plane and p0 is point on the plane.
    //
    // --> (origin - p0).n + t*(d.n) = 0
    // --> t = (p0 - origin).n / (d.n)
    const double directionDotN = dot(ray->direction, n);
    const double p0MinusOriginDotN = dot(subtractVectors(p0, ray->origin), n);

    // Line and plane are parallel --> no intersection point:
    if (directionDotN == 0)
        return false;

    // Negative intersect time --> intersects behind ray's position:
    const double intersectTime = (p0MinusOriginDotN / directionDotN);
    if (intersectTime < 0.0)
        return false;

    *hitTime = intersectTime;
    return true;
}


static inline bool solveQuadratic(double a, double b, double c, double *t1, double *t2)
{
    const double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0)
        return 0; // No roots.

    const double sqrtDiscriminant = sqrt(discriminant);

    *t1 = (-b - sqrtDiscriminant) / (2 * a);
    *t2 = (-b + sqrtDiscriminant) / (2 * a);

    return true;
}


static inline Ray transformRay(Ray *ray, Point3 center, Rotate3 *rotation)
{
    Vector3 newDir = inverseRotation(ray->direction, rotation);
    Point3 newOrigin = inverseRotation(subtractVectors(ray->origin, center), rotation);

    return makeRay(newOrigin, newDir);
}
