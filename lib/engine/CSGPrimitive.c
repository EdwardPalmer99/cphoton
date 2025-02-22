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
#include "utility/Utility.h"

// TODO: - move bounding boxes into separate file.

static AABB combineBoundingBoxes(const AABB *first, const AABB *second);
static bool CSGBoundingBoxTopLevel(Primitive *primitive, AABB *outputBox);

struct primitive_t *makeCSG(struct primitive_t *left, struct primitive_t *right, CSGOperation operation, bool isLeaf)
{
    CSGPrimitive *node = malloc(sizeof(CSGPrimitive));
    node->left = left;
    node->right = right;
    node->operation = operation;
    node->isLeaf = isLeaf;

    // Create our generic structure:
    Primitive *genericPrimitive = malloc(sizeof(Primitive));
    genericPrimitive->csgPrimitive = node;

    // NB: no material used.
    genericPrimitive->material = NULL;

    // Hookup methods.
    genericPrimitive->boundingBox = CSGBoundingBoxTopLevel;
    genericPrimitive->destructor = NULL; // NOT IMPLEMENTED.
    genericPrimitive->hit = NULL;

    return genericPrimitive;
}


Primitive *allocDemoCSGPrimitive(void)
{
    CSGPrimitive *demo = malloc(sizeof(CSGPrimitive));

    demo->operation = CSGDifference;

    Material *material1 = makeMetal(makeSolidTexture(color3(0, 1, 0)), 0);
    Material *material2 = makeMetal(makeSolidTexture(color3(1, 0, 0)), 0);

    demo->left = makeSphere(point3(0.5, 1, 0), 1, material1);
    demo->right = makeSphere(point3(-0.5, 1, 0), 1, material2);
    demo->isLeaf = true;

    // Hookup to a primitive.
    Primitive *primitive = malloc(sizeof(Primitive));

    primitive->csgPrimitive = demo;
    primitive->boundingBox = CSGBoundingBoxTopLevel;

    primitive->destructor = NULL;
    primitive->hit = NULL;

    // No material for CSG primitive.
    primitive->material = NULL;

    return primitive;
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
    CSGPrimitive *theCSGPrimitive = (CSGPrimitive *)primitive;

    AABB leftBox, rightBox;

    // Case 1: the top-level box is NOT a leaf node:
    if (!theCSGPrimitive->isLeaf)
    {
        // Ascent from bottom upwards, updating the AABB as we go along:
        (void)CSGBoundingBoxTopLevel(theCSGPrimitive->left, &leftBox);
        (void)CSGBoundingBoxTopLevel(theCSGPrimitive->right, &rightBox);
    }
    else // Case 2: the top-level box is NOT a leaf node:
    {
        // Create from the primitive's methods:
        theCSGPrimitive->left->boundingBox(theCSGPrimitive->left, &leftBox);
        theCSGPrimitive->right->boundingBox(theCSGPrimitive->right, &rightBox);
    }

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
