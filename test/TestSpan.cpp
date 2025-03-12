/**
 * @file TestCSGIntersectionTime.cpp
 * @author Edward Palmer
 * @date 2025-02-27
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "engine/Span.hpp"
#include <array>
#include <gtest/gtest.h>


TEST(Span, TestInsideInterval)
{
    Span span(1, 2);

    EXPECT_FALSE(span.insideInterval(1.0));
    EXPECT_TRUE(span.insideInterval(1.5));
    EXPECT_FALSE(span.insideInterval(2.0));

    EXPECT_FALSE(span.insideInterval(0.5));
    EXPECT_FALSE(span.insideInterval(2.5));
}


TEST(Span, TestIntervalsOverlap)
{
    Span i1(1, 2);

    Span i2 = i1;
    EXPECT_TRUE(i1.intervalsOverlap(i2));

    i2 = Span(1.5, 2.5);
    EXPECT_TRUE(i1.intervalsOverlap(i2));

    i2 = Span(0.5, 1.5);
    EXPECT_TRUE(i1.intervalsOverlap(i2));

    i2 = Span(0.5, 2.5);
    EXPECT_TRUE(i1.intervalsOverlap(i2));

    i2 = Span(1.0, 1.0);
    EXPECT_FALSE(i1.intervalsOverlap(i2));
}


TEST(Span, TestIsSubInterval)
{
    Span parent(1, 2);

    Span interval(1.1, 1.9);
    EXPECT_TRUE(parent.isSubInterval(interval));

    interval = Span(1, 2);
    EXPECT_TRUE(parent.isSubInterval(interval));

    interval = Span(0.5, 1.5);
    EXPECT_FALSE(parent.isSubInterval(interval));
}


TEST(Span, TestSubtractIntervals)
{
    std::array<Span, 2> results;

    Span original(1, 2);

    Span subtract(1, 2); // Total overlap --> nothing left (0).
    EXPECT_TRUE(original.differenceOperation(subtract, results) == 0);

    subtract = Span(0.5, 0.9); // No overlap --> returns (-1).
    EXPECT_TRUE(original.differenceOperation(subtract, results) == (-1));

    subtract = Span(0.5, 1.5);
    EXPECT_TRUE(original.differenceOperation(subtract, results) == 1);
    EXPECT_DOUBLE_EQ(results[0].entry.t, 1.5);
    EXPECT_DOUBLE_EQ(results[0].exit.t, 2.0);

    subtract = Span(1.5, 2.5);
    EXPECT_TRUE(original.differenceOperation(subtract, results) == 1);
    EXPECT_DOUBLE_EQ(results[0].entry.t, 1.0);
    EXPECT_DOUBLE_EQ(results[0].exit.t, 1.5);

    subtract = Span(1.2, 1.8);
    EXPECT_TRUE(original.differenceOperation(subtract, results) == 2);
    EXPECT_DOUBLE_EQ(results[0].entry.t, 1.0);
    EXPECT_DOUBLE_EQ(results[0].exit.t, 1.2);

    EXPECT_DOUBLE_EQ(results[1].entry.t, 1.8);
    EXPECT_DOUBLE_EQ(results[1].exit.t, 2.0);
}
