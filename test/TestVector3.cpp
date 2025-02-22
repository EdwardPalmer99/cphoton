/**
 * @file TestVector3.cpp
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gtest/gtest.h>

extern "C"
{
#include "utility/Vector3.h"
#include <math.h>
}

static void ExpectVectorEqual(Vector3 result, Vector3 expected);


TEST(Vector3, TestLengthSquared)
{
    Vector3 v = vector3(1.0, 2.0, 3.0);

    EXPECT_DOUBLE_EQ(lengthSquared(v), 14.0);
}


TEST(Vector3, TestVectorLength)
{
    Vector3 v = vector3(1.0, 2.0, 3.0);

    EXPECT_DOUBLE_EQ(vectorLength(v), sqrt(14.0));
}


TEST(Vector3, TestDot)
{
    Vector3 a = vector3(1.0, 2.0, 3.0);
    Vector3 b = vector3(2.0, 4.0, 6.0);

    EXPECT_DOUBLE_EQ(dot(a, b), 28.0);
}


TEST(Vector3, TestFlipVector)
{
    Vector3 a = vector3(1.0, 2.0, 3.0);
    Vector3 flipped = flipVector(a);

    EXPECT_DOUBLE_EQ(flipped.x, -a.x);
    EXPECT_DOUBLE_EQ(flipped.y, -a.y);
    EXPECT_DOUBLE_EQ(flipped.z, -a.z);
}


TEST(Vector3, TestScaleVector)
{
    Vector3 a = vector3(1.0, 2.0, 3.0);
    double scaleFactor = 2.0;

    Vector3 scaled = scaleVector(a, scaleFactor);

    EXPECT_DOUBLE_EQ(scaled.x, scaleFactor * a.x);
    EXPECT_DOUBLE_EQ(scaled.y, scaleFactor * a.y);
    EXPECT_DOUBLE_EQ(scaled.z, scaleFactor * a.z);
}


TEST(Vector3, TestUnitVector)
{
    Vector3 a = vector3(1.0, 2.0, 3.0); // length is 1 + 4 + 9 = 14.
    Vector3 unit = unitVector(a);

    const double invRoot14 = 1.0 / sqrt(14.0);

    EXPECT_DOUBLE_EQ(unit.x, invRoot14 * a.x);
    EXPECT_DOUBLE_EQ(unit.y, invRoot14 * a.y);
    EXPECT_DOUBLE_EQ(unit.z, invRoot14 * a.z);
}


TEST(Vector3, TestCross)
{
    Vector3 a = vector3(1.0, 2.0, 3.0);
    Vector3 b = vector3(3.0, 5.0, 8.0);

    Vector3 expected = vector3(1, 1, -1);
    Vector3 result = cross(a, b);

    ExpectVectorEqual(result, expected);
}


TEST(Vector3, TestSubtractVectors)
{
    Vector3 a = vector3(1.0, 2.0, 3.0);
    Vector3 b = vector3(3.0, 5.0, 8.0);

    ExpectVectorEqual(subtractVectors(a, b), vector3(-2, -3, -5));
}


TEST(Vector3, TestAddVectors)
{
    Vector3 a = vector3(1.0, 2.0, 3.0);
    Vector3 b = vector3(3.0, 5.0, 8.0);

    ExpectVectorEqual(addVectors(a, b), vector3(4, 7, 11));
}


TEST(Vector3, TestIsNearlyZero)
{
    Vector3 small = vector3(0.001, 0.001, 0.001);
    Vector3 zero = vector3(0, 0, 0);

    EXPECT_FALSE(isNearlyZero(small));
    EXPECT_TRUE(isNearlyZero(zero));
}


TEST(Vector3, TestRandomInt)
{
    EXPECT_EQ(randomInt(1, 1), 1);

    double result = randomInt(1, 3);
    EXPECT_TRUE(result >= 1 && result <= 3);
}


TEST(Vector3, TestMultiplyColors)
{
    Color3 red = color3(1, 0.1, 0.0);
    Color3 green = color3(0.1, 1, 0.0);

    ExpectVectorEqual(multiplyColors(red, green), color3(0.1, 0.1, 0.0));
}


TEST(Vector3, TestRandomUnitSphereVector)
{
    Vector3 result = randomUnitSphereVector();

    EXPECT_TRUE(lengthSquared(result) <= 1.0);
    EXPECT_TRUE(fabs(result.x) <= 1.0 && fabs(result.y) <= 1.0 && fabs(result.z) <= 1.0);
}


TEST(Vector3, TestRandomInUnitDisk)
{
    Vector3 result = randomInUnitDisk();

    EXPECT_TRUE(lengthSquared(result) <= 1.0);
    EXPECT_TRUE(fabs(result.x) <= 1.0 && fabs(result.y) <= 1.0 && fabs(result.z) == 0.0);
}


static void ExpectVectorEqual(Vector3 result, Vector3 expected)
{
    EXPECT_DOUBLE_EQ(result.x, expected.x);
    EXPECT_DOUBLE_EQ(result.y, expected.y);
    EXPECT_DOUBLE_EQ(result.z, expected.z);
}