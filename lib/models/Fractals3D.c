//
//  Fractals3D.c
//  CPhoton
//
//  Created by Edward on 05/02/2023.
//

#include "models/Fractals3D.h"
#include "utility/Utility.h"

#define kEpsilon 0.0001
#define kMaxIteration 128
#define kBailoutRadius 9.0

typedef struct fractal_t Fractal;
typedef struct quaternion_t Quaternion;


static inline bool isFractalHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit);
static inline bool fractalBoundingBox(Primitive *fractal, AABB *outputBox);
static void destructFractal(Primitive *primitive);

static inline Vector3 calculateFractalNormal(Point3 hitPoint, double (*DE)(Point3));
static inline double mandelbulb8DistanceEstimator(Point3 z);
static inline double quaternion3DistanceEstimator(Point3 p);


struct fractal_t
{
    Point3 center;
    double boundingRadius;               // Used to define AABB.
    double (*distanceEstimator)(Point3); // Distance estimator for the fractal.
};


// *** Quaternion Operations ***

struct quaternion_t
{
    double a, b, c, d; // q = a + b * i + c * j + d * k
};


// https://en.wikipedia.org/wiki/Quaternion
static inline Quaternion qAdd(Quaternion q1, Quaternion q2)
{
    Quaternion result = {.a = q1.a + q2.a, .b = q1.b + q2.b, .c = q1.c + q2.c, .d = q1.d + q2.d};

    return result;
}


static inline Quaternion qMultiply(Quaternion q1, Quaternion q2)
{
    Quaternion result = {.a = q1.a * q2.a - q1.b * q2.b - q1.c * q2.c - q1.d * q2.d,
                         .b = q1.a * q2.b + q1.b * q2.a + q1.c * q2.d - q1.d * q2.c,
                         .c = q1.a * q2.c - q1.b * q2.d + q1.c * q2.a + q1.d * q2.b,
                         .d = q1.a * q2.d + q1.b * q2.c - q1.c * q2.b + q1.d * q2.a};

    return result;
}


static inline double qLength2(Quaternion q1)
{
    return (q1.a * q1.a + q1.b * q1.b + q1.c * q1.c + q1.d * q1.d);
}


static inline Quaternion qSquare(Quaternion q1)
{
    return qMultiply(q1, q1);
}


static inline Quaternion qCube(Quaternion q1)
{
    return qMultiply(q1, qSquare(q1));
}


static inline bool fractalBoundingBox(Primitive *primitive, AABB *outputBox)
{
    Point3 center = primitive->fractal->center;
    const double r = primitive->fractal->boundingRadius;

    outputBox->min = point3(center.x - r, center.y - r, center.z - r);
    outputBox->max = point3(center.x + r, center.y + r, center.z + r);

    return true;
}


Primitive *makeMandelbulbFractal(Point3 center, Material *material)
{
    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive) return NULL;

    Fractal *fractal = malloc(sizeof(Fractal));
    if (!fractal)
    {
        free(primitive);
        return NULL;
    };

    fractal->center = center;
    fractal->boundingRadius = 2.0; // Estimate.
    fractal->distanceEstimator = mandelbulb8DistanceEstimator;

    primitive->fractal = fractal;
    primitive->hit = isFractalHit;
    primitive->boundingBox = fractalBoundingBox;
    primitive->destructor = destructFractal;
    primitive->material = material;

    material->incrementRefCounter(material);

    return primitive;
}


Primitive *makeQuaternionFractal(Point3 center, Material *material)
{
    Primitive *primitive = malloc(sizeof(Primitive));
    if (!primitive) return NULL;

    Fractal *fractal = malloc(sizeof(Fractal));
    if (!fractal)
    {
        free(primitive);
        return NULL;
    };

    fractal->center = center;
    fractal->boundingRadius = 3;
    fractal->distanceEstimator = quaternion3DistanceEstimator;

    primitive->fractal = fractal;
    primitive->hit = isFractalHit;
    primitive->boundingBox = fractalBoundingBox;
    primitive->destructor = destructFractal;
    primitive->material = material;

    material->incrementRefCounter(material);

    return primitive;
}


static void destructFractal(Primitive *primitive)
{
    if (primitive)
    {
        if (primitive->fractal) free(primitive->fractal);

        primitive->material->decrementRefCounter(primitive->material);
        free(primitive);
    }
}


// *** Distance Estimators ***

/// Returns the closest point to a point in the Mandelbrot set.
static inline double mandelbulb8DistanceEstimator(Point3 c)
{
    Vector3 z = c;

    const int kPower = 8;

    double dr = 1.0;
    double r = 0.0;

    for (int iter = 0; iter < kMaxIteration; iter++)
    {
        if ((r = vectorLength(z)) > kBailoutRadius) break;

        // Length of the running derivative, dr:
        dr = kPower * pow(r, kPower - 1) * dr + 1.0;

        // Convert to spherical polar coordinates:
        const double rPowerN = pow(r, kPower);

        const double nPhi = kPower * atan2(z.y, z.x);
        const double nTheta = kPower * acos(z.z / r);

        // z^n formula:
        Vector3 zPowerN = {rPowerN * sin(nTheta) * cos(nPhi), rPowerN * sin(nTheta) * sin(nPhi), rPowerN * cos(nTheta)};

        // z -> z^n + c
        z = addVectors(zPowerN, c);
    }

    return (0.5 * log(r) * r / dr);
}


static inline double quaternion3DistanceEstimator(Point3 p)
{
    Quaternion z = {.a = p.x, .b = p.y, .c = p.z, .d = 0.0};
    Quaternion zc = {0.1, 0.1, 0.1, -1};

    double dr2 = 1.0;
    double r2 = 0.0;

    for (int iter = 0; iter < 200; iter++)
    {
        dr2 *= 9.0 * qLength2(qSquare(z)); // Use squares to avoid sqrt.

        z = qAdd(qCube(z), zc); // z -> z^3 + c

        r2 = qLength2(z);

        if (r2 > kBailoutRadius) break;
    }

    return (0.25 * log(r2) * sqrt(r2 / dr2));
}


static inline Vector3 calculateFractalNormal(Point3 hitPoint, double (*DE)(Point3))
{
    Vector3 dx = vector3(kEpsilon, 0, 0);
    Vector3 dy = vector3(0, kEpsilon, 0);
    Vector3 dz = vector3(0, 0, kEpsilon);

    double DEx = DE(addVectors(hitPoint, dx)) - DE(subtractVectors(hitPoint, dx));
    double DEy = DE(addVectors(hitPoint, dy)) - DE(subtractVectors(hitPoint, dy));
    double DEz = DE(addVectors(hitPoint, dz)) - DE(subtractVectors(hitPoint, dz));

    return unitVector(vector3(DEx, DEy, DEz));
}


/// Ray-marching approach.
bool isFractalHit(Primitive *primitive, Ray *ray, double tmin, double tmax, HitRec *hit)
{
    Fractal *fractal = primitive->fractal;
    double (*DE)(Vector3) = fractal->distanceEstimator;

    Point3 tOrigin = subtractVectors(ray->origin, fractal->center);

    const double invDirMagnitude = 1.0 / vectorLength(ray->direction);

    Vector3 unitDir = scaleVector(ray->direction, invDirMagnitude);

    double totalDistance = tmin;
    bool converged = false;

    Vector3 normal = {0};

    for (int iter = 0; iter < kMaxIteration; iter++)
    {
        Point3 pt = addVectors(tOrigin, scaleVector(unitDir, totalDistance));

        const double distance = DE(pt);

        totalDistance += min(distance, 0.2);

        if (distance < kEpsilon)
        {
            converged = true;
            normal = calculateFractalNormal(pt, DE);
            break;
        }
    }

    if (!converged) return false;

    const bool frontFace = (dot(ray->direction, normal) < 0.0);

    hit->t = totalDistance * invDirMagnitude;
    hit->hitPt = pointAtTime(ray, hit->t);
    hit->normal = frontFace ? normal : flipVector(normal);
    hit->material = primitive->material;
    hit->frontFace = frontFace;

    return true;
}
