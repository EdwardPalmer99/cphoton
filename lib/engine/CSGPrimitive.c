/**
 * @file CSGPrimitive.c
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "engine/CSGPrimitive.h"
#include "engine/HitRec.h"
#include "engine/Material.h"
#include "engine/Primitive.h"
#include "engine/SpanList.h"
#include "engine/Texture.h"
#include "logger/Logger.h"
#include "utility/Utility.h"

// TODO: - rotation should be a Primitive method. Does nothing if not implemented (Primitive class can implement it as
// base). But we can call this rotation method to modify all primitives in our CSG so we can do rotations of complex
// objects.

// TODO: - move bounding boxes into separate file.
static AABB combineBoundingBoxes(const AABB *first, const AABB *second);

static bool computeCSGIntersectionTimes(Primitive *primitive, Ray *ray, SpanList *result);

// Methods
//
static bool CSGBoundingBoxTopLevel(Primitive *primitive, AABB *outputBox);
static void deallocCSG(Primitive *primitive);

static bool hit(Primitive *thePrimitive, Ray *ray, double tmin, double tmax, HitRec *hit);


// TODO: - rename to allocCSG
struct primitive_t *makeCSG(struct primitive_t *left, struct primitive_t *right, CSGOperation operation)
{
    CSGPrimitive *node = malloc(sizeof(CSGPrimitive));
    node->left = left;
    node->right = right;
    node->operation = operation;

    // Create our generic structure:
    Primitive *primitive = malloc(sizeof(Primitive));
    primitive->csg = node;

    // Hookup methods.
    primitive->destructor = deallocCSG;
    primitive->hit = hit;
    primitive->boundingBox = CSGBoundingBoxTopLevel;
    primitive->intersectionTimes = computeCSGIntersectionTimes;

    // NB: no material used.
    primitive->material = NULL;

    return primitive;
}

/**
 * Destroys allocated memory for CSG and for any other primitives/CSG primitives in tree.
 */
static void deallocCSG(Primitive *primitive)
{
    if (!primitive) return;

    CSGPrimitive *CSG = primitive->csg;
    if (CSG)
    {
        if (CSG->left) CSG->left->destructor(CSG->left);
        if (CSG->right) CSG->right->destructor(CSG->right);

        free(CSG);
    }

    free(primitive);
}


/**
 * Calculate the bounding box for a CSG primitive.
 *
 * We want to call this recursively. At the leaf nodes, we generate a bounding box which includes both primitives we are
 * going to be doing an operation with. We then pass this up the tree, creating a box which encompasses everything
 * inside.
 */
static bool CSGBoundingBoxTopLevel(Primitive *primitive, AABB *outputBox)
{
    CSGPrimitive *theCSGPrimitive = primitive->csg;

    AABB leftBox, rightBox;

    // Create from the primitive or CSG primitive's methods:
    theCSGPrimitive->left->boundingBox(theCSGPrimitive->left, &leftBox);
    theCSGPrimitive->right->boundingBox(theCSGPrimitive->right, &rightBox);

    // Combine to generate large bounding box encompassing everything in the tree. This can be made more efficient!
    *outputBox = combineBoundingBoxes(&leftBox, &rightBox);
    return true;
}


// Returns a new bounding box which combines the first and second.
static AABB combineBoundingBoxes(const AABB *first, const AABB *second)
{
    AABB result;

    result.min.x = min(first->min.x, second->min.x);
    result.min.y = min(first->min.y, second->min.y);
    result.min.z = min(first->min.z, second->min.z);

    result.max.x = max(first->max.x, second->max.x);
    result.max.y = max(first->max.y, second->max.y);
    result.max.z = max(first->max.z, second->max.z);

    return result;
}

/**
 *
 * @param primitive The CSG primitive
 * @param ray       The ray we are intersecting
 * @param tmin      Min time for intersection (usually zero)
 * @param tmax      Max time for intersection (usually infinity)
 * @return          A SpanList record pointer or NULL if no intersections
 */
static bool computeCSGIntersectionTimes(Primitive *primitive, Ray *ray, SpanList *result)
{
    if (!primitive) return false;

    CSGPrimitive *CSG = primitive->csg;

    // Tree this as a tree. We want to compute this on any children and work our way up.
    SpanList leftIntervals, rightIntervals;

    // TODO: - can use bool results to calculate if we need to perform operation.
    bool leftOK = CSG->left->intersectionTimes(CSG->left, ray, &leftIntervals);
    bool rightOK = CSG->right->intersectionTimes(CSG->right, ray, &rightIntervals);

    // TODO: - refer to notes here https://www.doc.ic.ac.uk/~dfg/graphics/graphics2008/GraphicsSlides10.pdf
    // we can optimize for certain operations if empty list returned.

    switch (CSG->operation)
    {
        case CSGSubtraction:
        {
            return subtractSpanLists(leftOK ? &leftIntervals : NULL, rightOK ? &rightIntervals : NULL, result);
        }
        default:
        {
            // NOT currently supported.
            LogFailed("CSG operation type not currently supported");
            exit(EXIT_FAILURE);
        }
    }
}


/**
 * Returns true if the ray intersects with the primitive in interval [tmin, tmax]. If we have a hit, we populate hte
 * HitRec structure.
 */
static bool hit(Primitive *thePrimitive, Ray *ray, double tmin, double tmax, HitRec *hit)
{
    // Currently ignore tmin, tmax for testing...
    SpanList hitTimes;

    if (!thePrimitive->intersectionTimes(thePrimitive, ray, &hitTimes))
    {
        return false; // Hit nothing.
    }

    // TODO: - will need to do some logic based on which normals to preserve, materials, etc based on CSG type.

    // Find the first positive t (either entry or exit).
    HitRec *lowestT = NULL;

    for (int i = 0; i < hitTimes.n; ++i)
    {
        SpanRec *span = (hitTimes.intervals + i);

        HitRec *best = (span->entry.t > tmin) ? &span->entry : &span->exit;

        if (best->t > 0 && (!lowestT || lowestT->t > best->t))
        {
            lowestT = best;
        }
    }

    if (lowestT)
    {
        *hit = *lowestT;
    }

    return (lowestT != NULL);
}
