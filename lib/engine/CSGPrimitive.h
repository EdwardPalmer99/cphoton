/**
 * @file CSGPrimitive.h
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */


#pragma once
#include <stdbool.h>

// Forward declaration.
struct primitive_t;

typedef enum
{
    //   CSGIntersection,
    CSGUnion,
    CSGAddition,
    CSGSubtraction,
    CSGDifference
} CSGOperation;

// TODO: - this structure will need to go inside the Primitive * structure and we need to implement
// its methods.

/**
 * Structure for a CSG primitive.
 *
 * If this is a leaf-node, there will be no further CSG Primitives and we just read the left and right primitives.
 * Otherwise, we read the CSG primitives and continue down the tree.
 */
typedef struct CSGPrimitive_t
{
    CSGOperation operation;

    // These can be further CSGPrimitives or other Primitives.
    struct primitive_t *left;
    struct primitive_t *right;

} CSGPrimitive;


/**
 * Constructs a new CSGPrimitive for performing CSG operations.
 *
 * @param left      The CSGPrimitive/Primitive to operate on LHS of tree.
 * @param right     The CSGPrimitive/Primitive to operate on RHS of tree.
 * @param operation The CSG operation to perform.
 * @return A new CSG primitive
 */
struct primitive_t *makeCSG(struct primitive_t *left, struct primitive_t *right, CSGOperation operation);
