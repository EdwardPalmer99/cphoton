/**
 * @file TestSpanList.cpp
 * @author Edward Palmer
 * @date 2025-02-27
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "engine/Span.hpp"
#include <gtest/gtest.h>
#include <iostream>

using SpanList = Span::SpanList;

/* Result: two intervals */
TEST(SpanList, TestSimpleSubsetSubtraction)
{
    SpanList original = {Span(1, 5)};
    SpanList subtracted = {Span(2, 3)};

    SpanList result;
    ASSERT_EQ(Span::subtractSpanLists(original, subtracted, result), 2);

    EXPECT_DOUBLE_EQ(result[0].entry.t, 1.0);
    EXPECT_DOUBLE_EQ(result[0].exit.t, 2.0);

    EXPECT_DOUBLE_EQ(result[1].entry.t, 3.0);
    EXPECT_DOUBLE_EQ(result[1].exit.t, 5.0);
}

// /**
//  * Span 1:   1 --- 3   4 --- 6
//  * Span 2:      2 ------- 5
//  * Result:   1--2         5--6
//  */
TEST(SpanList, Test2SpansMinus1Span)
{
    SpanList original = {Span(1, 3), Span(4, 6)};
    SpanList subtracted = {Span(2, 5)};

    SpanList result;
    ASSERT_TRUE(Span::subtractSpanLists(original, subtracted, result));

    EXPECT_EQ(result.size(), 2);
    EXPECT_TRUE(result[0].entry.t == 1 && result[0].exit.t == 2);
    EXPECT_TRUE(result[1].entry.t == 5 && result[1].exit.t == 6);
}

/**
 * Span 1:   1 --- 3   4 --- 7
 * Span 2:  0 -- 2       5-6
 * Result:      2-3    4-5  6-7
 */
TEST(SpanList, Test2SpansMinus2Spans)
{
    SpanList original = {Span(1, 3), Span(4, 7)};
    SpanList subtracted = {Span(0, 2), Span(5, 6)};

    SpanList result;
    ASSERT_TRUE(Span::subtractSpanLists(original, subtracted, result));

    EXPECT_EQ(result.size(), 3);
    EXPECT_TRUE(result[0].entry.t == 2 && result[0].exit.t == 3);
    EXPECT_TRUE(result[1].entry.t == 4 && result[1].exit.t == 5);
    EXPECT_TRUE(result[2].entry.t == 6 && result[2].exit.t == 7);
}

/* Result: original subset */
TEST(SpanList, TestNoOverlapSubtraction)
{
    SpanList original = {Span(1, 5)};
    SpanList subtracted = {Span(0, 0.5)};

    SpanList result;
    ASSERT_TRUE(Span::subtractSpanLists(original, subtracted, result));

    ASSERT_TRUE(result.size() == 1);
    EXPECT_TRUE(result[0].entry.t == original[0].entry.t);
    EXPECT_TRUE(result[0].exit.t == original[0].exit.t);
}

/* Result: expect false */
TEST(SpanList, TestCompleteOverlapSubtraction)
{
    SpanList original = {Span(1, 5)};
    SpanList subtracted = {Span(1, 5)};

    SpanList result;
    EXPECT_FALSE(Span::subtractSpanLists(original, subtracted, result));
}
