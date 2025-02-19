#include "models/MengerCube.h"

#define kInitialCubeStackCapacity 8

/*******************************************************************************
 Reference: https://en.wikipedia.org/wiki/Menger_sponge
*******************************************************************************/

typedef struct
{
    int8_t iteration;
    double sideLen;
    Point3 center;
} MengerCube;


typedef struct
{
    int ncubes;
    int capacity;
    MengerCube *cubes;
} CubeStack;


static CubeStack *makeCubeStack(void);
static void freeCubeStack(CubeStack *stack);
static MengerCube popCube(CubeStack *stack);
static void pushCube(CubeStack *stack, MengerCube *cube);
static bool subdivideCube(MengerCube *subCubes, MengerCube *parent);
static MengerCube makeMengerCube(short int iter, double len, double x, double y, double z);


Primitive *makeMengerSponge(int8_t n, Point3 center, double sideLength, Material *material)
{
    if (n < 0 || n > 6 || sideLength <= 0.0 || !material) return NULL;

    const int numOutputCubes = pow(20, n);

    CubeStack *inputStack = makeCubeStack();
    if (!inputStack) return NULL;

    Primitive **objects = malloc(sizeof(Primitive *) * numOutputCubes);
    if (!objects)
    {
        freeCubeStack(inputStack);
        return NULL;
    }

    int numObjectsAdded = 0;
    Vector3 noRotation = zeroVector();

    // Make initial cube for iteration 0 and add to stack:
    MengerCube initialCube = makeMengerCube(0, sideLength, 0, 0, 0);
    pushCube(inputStack, &initialCube);

    // 20 smaller cubes made from a single larger cube:
    MengerCube smallerCubes[20];

    while (inputStack->ncubes > 0)
    {
        MengerCube poppedCube = popCube(inputStack);

        if (poppedCube.iteration == n)
        {
            Point3 objectCenter = addVectors(poppedCube.center, center);
            objects[numObjectsAdded++] = makeCube(objectCenter, noRotation, poppedCube.sideLen, material);
            continue;
        }

        // Subdivide into 20 children cubes excluding center of popped cube and
        // center of its 6 faces. Push these back onto the stack with an
        // incremented cube count.
        subdivideCube(smallerCubes, &poppedCube);

        for (int iCube = 0; iCube < 20; iCube++)
        {
            pushCube(inputStack, smallerCubes + iCube);
        }
    }

    // Create BVHNode from primitives:
    Primitive *node = makeBVHNode(objects, 0, numObjectsAdded);

    // Cleanup:
    freeCubeStack(inputStack);
    free(objects);

    return node;
}


static MengerCube makeMengerCube(short int iter, double len, double x, double y, double z)
{
    MengerCube cube = {.iteration = iter, .sideLen = len, .center = {x, y, z}};

    return cube;
}


static CubeStack *makeCubeStack(void)
{
    CubeStack *stack = malloc(sizeof(CubeStack));
    if (!stack) return NULL;

    stack->ncubes = 0;
    stack->capacity = kInitialCubeStackCapacity;

    stack->cubes = malloc(sizeof(MengerCube) * kInitialCubeStackCapacity);
    if (!stack->cubes)
    {
        free(stack);
        return NULL;
    }

    return stack;
}


static void freeCubeStack(CubeStack *stack)
{
    if (stack)
    {
        if (stack->cubes) free(stack->cubes);
        free(stack);
    }
}


static void pushCube(CubeStack *stack, MengerCube *cube)
{
    if (stack->ncubes >= stack->capacity)
    {
        const int newCapacity = sizeof(MengerCube) * stack->capacity * 2;
        void *newBlock = realloc(stack->cubes, newCapacity);

        if (newBlock)
        {
            stack->capacity *= 2;
            stack->cubes = newBlock;
        }
        else
        {
            fprintf(stderr, "LogLevelError: could not add cube to stack.\n");
        }
    }

    memcpy(stack->cubes + stack->ncubes++, cube, sizeof(MengerCube));
}


static MengerCube popCube(CubeStack *stack)
{
    return stack->cubes[--stack->ncubes];
}


/// Subdivides a parent cube into 20 children cubes. These are stored in a
/// destination array supplied to the function. We don't add the center cube
/// and the center of each face.
static bool subdivideCube(MengerCube *subCubes, MengerCube *parent)
{
    if (!subCubes || !parent) return false;

    const Point3 center = parent->center;

    // Add cubes for top-face excluding center hole:
    short int nextIter = parent->iteration + 1;
    double sideLen = parent->sideLen / 3.0;

    // Clockwise:
    typedef enum
    {
        kTopCubes = 0,
        kMiddleCubes = 1,
        kBottomCubes = 2
    } CubeGeneration;

    int nsubCubes = 0; // Should be 20!

    double cubeCenterY;

    for (CubeGeneration mode = kTopCubes; mode <= kBottomCubes; mode++)
    {
        if (mode == kTopCubes)
            cubeCenterY = center.y + sideLen;
        else if (mode == kMiddleCubes)
            cubeCenterY = center.y;
        else
            cubeCenterY = center.y - sideLen;

        // Four corners (going clockwise):
        subCubes[nsubCubes++] = makeMengerCube(nextIter, sideLen, center.x - sideLen, cubeCenterY, center.z + sideLen);
        subCubes[nsubCubes++] = makeMengerCube(nextIter, sideLen, center.x - sideLen, cubeCenterY, center.z - sideLen);
        subCubes[nsubCubes++] = makeMengerCube(nextIter, sideLen, center.x + sideLen, cubeCenterY, center.z + sideLen);
        subCubes[nsubCubes++] = makeMengerCube(nextIter, sideLen, center.x + sideLen, cubeCenterY, center.z - sideLen);

        if (mode != kMiddleCubes)
        {
            subCubes[nsubCubes++] = makeMengerCube(nextIter, sideLen, center.x - sideLen, cubeCenterY, center.z);
            subCubes[nsubCubes++] = makeMengerCube(nextIter, sideLen, center.x + sideLen, cubeCenterY, center.z);

            subCubes[nsubCubes++] = makeMengerCube(nextIter, sideLen, center.x, cubeCenterY, center.z - sideLen);
            subCubes[nsubCubes++] = makeMengerCube(nextIter, sideLen, center.x, cubeCenterY, center.z + sideLen);
        }
    }

    return true;
}
