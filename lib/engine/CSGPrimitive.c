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
#include "engine/Texture.h"
#include "logger/Logger.h"
#include "utility/Utility.h"

// TODO: - move bounding boxes into separate file.
static AABB combineBoundingBoxes(const AABB *first, const AABB *second);

// Methods
//
static bool CSGBoundingBoxTopLevel(Primitive *primitive, AABB *outputBox);
static void CSGDestructor(Primitive *primitive);


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
    primitive->destructor = CSGDestructor;
    primitive->hit = NULL; // TODO: - not hookedup yet.
    primitive->boundingBox = CSGBoundingBoxTopLevel;

    // NB: no material used.
    primitive->material = NULL;

    return primitive;
}

/**
 * Destroys allocated memory for CSG and for any other primitives/CSG primitives in tree.
 */
static void CSGDestructor(Primitive *primitive)
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
 * Returns true if the ray intersects with the primitive in interval [tmin, tmax]. If we have a hit, we populate hte
 * HitRec structure.
 */
// bool hit(Primitive *thePrimitive, Ray *theRay, double tmin, double tmax, HitRec *hit)
// {
// }
