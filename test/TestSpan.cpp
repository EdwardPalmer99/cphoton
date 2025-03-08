/**
 * @file TestCSGIntersectionTime.cpp
 * @author Edward Palmer
 * @date 2025-02-27
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "engine/Span.hpp"
#include <gtest/gtest.h>

// TEST(Span, TestInsideInterval)
// {
//     SpanRec intersection = {.entry.t = 1, .exit.t = 2};

//     EXPECT_TRUE(insideInterval(&intersection, 1.0));
//     EXPECT_TRUE(insideInterval(&intersection, 1.5));
//     EXPECT_TRUE(insideInterval(&intersection, 2.0));

//     EXPECT_FALSE(insideInterval(&intersection, 0.5));
//     EXPECT_FALSE(insideInterval(&intersection, 2.5));

//     EXPECT_FALSE(insideInterval(NULL, 1.0));
// }


// TEST(Span, TestIntervalsOverlap)
// {
//     SpanRec i1 = {.entry.t = 1, .exit.t = 2};

//     SpanRec i2 = i1;
//     EXPECT_TRUE(intervalsOverlap(&i1, &i2));

//     i2 = {.entry.t = 1.5, .exit.t = 2.5};
//     EXPECT_TRUE(intervalsOverlap(&i1, &i2));

//     i2 = {.entry.t = 0.5, .exit.t = 1.5};
//     EXPECT_TRUE(intervalsOverlap(&i1, &i2));

//     i2 = {.entry.t = 0.5, .exit.t = 2.5};
//     EXPECT_TRUE(intervalsOverlap(&i1, &i2));

//     i2 = {.entry.t = 1.0, .exit.t = 1.0};
//     EXPECT_TRUE(intervalsOverlap(&i1, &i2));

//     EXPECT_FALSE(intervalsOverlap(&i1, NULL));
// }


// TEST(Span, TestIsSubInterval)
// {
//     SpanRec parent = {.entry.t = 1, .exit.t = 2};

//     SpanRec interval = {.entry.t = 1.1, .exit.t = 1.9};
//     EXPECT_TRUE(isSubInterval(&parent, &interval));

//     interval = {.entry.t = 1, .exit.t = 2};
//     EXPECT_TRUE(isSubInterval(&parent, &interval));

//     interval = {.entry.t = 0.5, .exit.t = 1.5};
//     EXPECT_FALSE(isSubInterval(&parent, &interval));
// }


// TEST(Span, TestSubtractIntervals)
// {
//     SpanRec results[2];

//     SpanRec original = {.entry.t = 1, .exit.t = 2};

//     SpanRec subtract = {.entry.t = 1, .exit.t = 2}; // Total overlap --> nothing left (0).
//     EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == 0);

//     subtract = {.entry.t = 0.5, .exit.t = 0.9}; // No overlap --> returns (-1).
//     EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == (-1));

//     subtract = {.entry.t = 0.5, .exit.t = 1.5};
//     EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == 1);
//     EXPECT_DOUBLE_EQ(results->entry.t, 1.5);
//     EXPECT_DOUBLE_EQ(results->exit.t, 2.0);

//     subtract = {.entry.t = 1.5, .exit.t = 2.5};
//     EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == 1);
//     EXPECT_DOUBLE_EQ(results->entry.t, 1.0);
//     EXPECT_DOUBLE_EQ(results->exit.t, 1.5);

//     subtract = {.entry.t = 1.2, .exit.t = 1.8};
//     EXPECT_TRUE(subtractIntervals(&original, &subtract, results) == 2);
//     EXPECT_DOUBLE_EQ(results[0].entry.t, 1.0);
//     EXPECT_DOUBLE_EQ(results[0].exit.t, 1.2);

//     EXPECT_DOUBLE_EQ(results[1].entry.t, 1.8);
//     EXPECT_DOUBLE_EQ(results[1].exit.t, 2.0);
// }
