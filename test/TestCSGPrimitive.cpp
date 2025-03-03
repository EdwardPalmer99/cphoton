/**
 * @file TestCSGPrimitive.cpp
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "engine/primitives/CSGNode.hpp"
#include "engine/primitives/Sphere.hpp"
#include <gtest/gtest.h>

static Primitive *BuildLeafCSGFromSpherePair(Point3 pt1, Point3 pt2, double radius);


TEST(CSGPrimitive, TestBoundingBoxWithLeafCSG)
{
    // Create two spheres with overlap:
    Primitive *theCSG = BuildLeafCSGFromSpherePair(point3(0.5, 1, 0), point3(-0.5, 1, 0), 1.0);
    ASSERT_TRUE(theCSG != nullptr);

    // The bounding box should be a box that encompasses both spheres.
    AABB boundingBox;
    ASSERT_TRUE(theCSG->boundingBox(&boundingBox));

    EXPECT_DOUBLE_EQ(boundingBox.min.x, -1.5);
    EXPECT_DOUBLE_EQ(boundingBox.min.y, 0.0);
    EXPECT_DOUBLE_EQ(boundingBox.min.z, -1.0);

    EXPECT_DOUBLE_EQ(boundingBox.max.x, 1.5);
    EXPECT_DOUBLE_EQ(boundingBox.max.y, 2.0);
    EXPECT_DOUBLE_EQ(boundingBox.max.z, 1.0);

    delete theCSG;
}


TEST(CSGPrimitive, TestBoundingBoxWithNonLeafCSG)
{
    Primitive *leafCSG1 = BuildLeafCSGFromSpherePair(point3(0.5, 1, 0), point3(-0.5, 1, 0), 1.0);
    Primitive *leafCSG2 = BuildLeafCSGFromSpherePair(point3(0.5, 3, 0), point3(-0.5, 3, 0), 1.0);

    Primitive *theCSG = new CSGNode(leafCSG1, leafCSG2, CSGNode::CSGDifference);
    ASSERT_TRUE(theCSG != nullptr);

    // The bounding box should be include both bounding boxes.
    AABB boundingBox;
    ASSERT_TRUE(theCSG->boundingBox(&boundingBox));

    EXPECT_DOUBLE_EQ(boundingBox.min.x, -1.5);
    EXPECT_DOUBLE_EQ(boundingBox.min.y, 0.0);
    EXPECT_DOUBLE_EQ(boundingBox.min.z, -1.0);

    EXPECT_DOUBLE_EQ(boundingBox.max.x, 1.5);
    EXPECT_DOUBLE_EQ(boundingBox.max.y, 4.0);
    EXPECT_DOUBLE_EQ(boundingBox.max.z, 1.0);

    delete theCSG;
}


/// Constructs a leaf-CSG node from two overlapping spheres.
static Primitive *BuildLeafCSGFromSpherePair(Point3 pt1, Point3 pt2, double radius)
{
    Material *material1 = makeMetal(makeSolidTexture(color3(0, 1, 0)), 0);
    Material *material2 = makeMetal(makeSolidTexture(color3(1, 0, 0)), 0);

    Primitive *sphere1 = new Sphere(pt1, radius, material1);
    Primitive *sphere2 = new Sphere(pt2, radius, material2);

    return new CSGNode(sphere1, sphere2, CSGNode::CSGDifference);
}