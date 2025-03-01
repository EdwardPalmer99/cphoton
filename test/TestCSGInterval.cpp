/**
 * @file TestCSGIntersectionTime.cpp
 * @author Edward Palmer
 * @date 2025-02-27
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <gtest/gtest.h>

extern "C"
{
#include "engine/CSGInterval.h"
}

TEST(CSGInterval, TestInsideInterval)
{
    CSGInterval intersection = {.tEnter = 1, .tExit = 2};

    EXPECT_TRUE(insideInterval(&intersection, 1.0));
    EXPECT_TRUE(insideInterval(&intersection, 1.5));
    EXPECT_TRUE(insideInterval(&intersection, 2.0));

    EXPECT_FALSE(insideInterval(&intersection, 0.5));
    EXPECT_FALSE(insideInterval(&intersection, 2.5));

    EXPECT_FALSE(insideInterval(NULL, 1.0));
}


TEST(CSGInterval, TestIntervalsOverlap)
{
    CSGInterval i1 = {.tEnter = 1, .tExit = 2};

    CSGInterval i2 = i1;
    EXPECT_TRUE(intervalsOverlap(&i1, &i2));

    i2 = {.tEnter = 1.5, .tExit = 2.5};
    EXPECT_TRUE(intervalsOverlap(&i1, &i2));

    i2 = {.tEnter = 0.5, .tExit = 1.5};
    EXPECT_TRUE(intervalsOverlap(&i1, &i2));

    i2 = {.tEnter = 0.5, .tExit = 2.5};
    EXPECT_TRUE(intervalsOverlap(&i1, &i2));

    i2 = {.tEnter = 1.0, .tExit = 1.0};
    EXPECT_TRUE(intervalsOverlap(&i1, &i2));

    EXPECT_FALSE(intervalsOverlap(&i1, NULL));
}


TEST(CSGInterval, TestIsSubInterval)
{
    CSGInterval parent = {.tEnter = 1, .tExit = 2};

    CSGInterval interval = {.tEnter = 1.1, .tExit = 1.9};
    EXPECT_TRUE(isSubInterval(&parent, &interval));

    interval = {.tEnter = 1, .tExit = 2};
    EXPECT_TRUE(isSubInterval(&parent, &interval));

    interval = {.tEnter = 0.5, .tExit = 1.5};
    EXPECT_FALSE(isSubInterval(&parent, &interval));
}


TEST(CSGInterval, TestSubtractIntervals)
{
    CSGInterval results[2];

    CSGInterval original = {.tEnter = 1, .tExit = 2};

    CSGInterval subtract = {.tEnter = 1, .tExit = 2};
    EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == 0);

    subtract = {.tEnter = 0.5, .tExit = 0.9};
    EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == 0);

    subtract = {.tEnter = 0.5, .tExit = 1.5};
    EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == 1);
    EXPECT_DOUBLE_EQ(results->tEnter, 1.5);
    EXPECT_DOUBLE_EQ(results->tExit, 2.0);

    subtract = {.tEnter = 1.5, .tExit = 2.5};
    EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == 1);
    EXPECT_DOUBLE_EQ(results->tEnter, 1.0);
    EXPECT_DOUBLE_EQ(results->tExit, 1.5);

    subtract = {.tEnter = 1.2, .tExit = 1.8};
    EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == 2);
    EXPECT_DOUBLE_EQ(results[0].tEnter, 1.0);
    EXPECT_DOUBLE_EQ(results[0].tExit, 1.2);

    EXPECT_DOUBLE_EQ(results[1].tEnter, 1.8);
    EXPECT_DOUBLE_EQ(results[1].tExit, 2.0);
}
