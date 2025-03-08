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


/* Result: two intervals */
// TEST(SpanList, TestSimpleSubsetSubtraction)
// {
//     SpanList original = {.n = 1, .intervals[0] = {.entry.t = 1, .exit.t = 5}};
//     SpanList subtracted = {.n = 1, .intervals[0] = {.entry.t = 2, .exit.t = 3}};

//     SpanList result;
//     ASSERT_TRUE(subtractSpanLists(&original, &subtracted, &result));

//     ASSERT_TRUE(result.n == 2);
//     EXPECT_TRUE(result.intervals[0].entry.t == 1 && result.intervals[0].exit.t == 2);
//     EXPECT_TRUE(result.intervals[1].entry.t == 3 && result.intervals[1].exit.t == 5);
// }

// /**
//  * Span 1:   1 --- 3   4 --- 6
//  * Span 2:      2 ------- 5
//  * Result:   1--2         5--6
//  */
// TEST(SpanList, Test2SpansMinus1Span)
// {
//     SpanList original = {.n = 2, .intervals = {{.entry.t = 1, .exit.t = 3}, {.entry.t = 4, .exit.t = 6}}};
//     SpanList subtracted = {.n = 1, .intervals[0] = {.entry.t = 2, .exit.t = 5}};

//     SpanList result;
//     ASSERT_TRUE(subtractSpanLists(&original, &subtracted, &result));

//     EXPECT_EQ(result.n, 2);
//     EXPECT_TRUE(result.intervals[0].entry.t == 1 && result.intervals[0].exit.t == 2);
//     EXPECT_TRUE(result.intervals[1].entry.t == 5 && result.intervals[1].exit.t == 6);
// }

// /**
//  * Span 1:   1 --- 3   4 --- 7
//  * Span 2:  0 -- 2       5-6
//  * Result:      2-3    4-5  6-7
//  */
// TEST(SpanList, Test2SpansMinus2Spans)
// {
//     SpanList original = {.n = 2, .intervals = {{.entry.t = 1, .exit.t = 3}, {.entry.t = 4, .exit.t = 7}}};
//     SpanList subtracted = {.n = 2, .intervals = {{.entry.t = 0, .exit.t = 2}, {.entry.t = 5, .exit.t = 6}}};

//     SpanList result;
//     ASSERT_TRUE(subtractSpanLists(&original, &subtracted, &result));

//     EXPECT_EQ(result.n, 3);
//     EXPECT_TRUE(result.intervals[0].entry.t == 2 && result.intervals[0].exit.t == 3);
//     EXPECT_TRUE(result.intervals[1].entry.t == 4 && result.intervals[1].exit.t == 5);
//     EXPECT_TRUE(result.intervals[2].entry.t == 6 && result.intervals[2].exit.t == 7);
// }

// /* Result: original subset */
// TEST(SpanList, TestNoOverlapSubtraction)
// {
//     SpanList original = {.n = 1, .intervals[0] = {.entry.t = 1, .exit.t = 5}};
//     SpanList subtracted = {.n = 1, .intervals[0] = {.entry.t = 0, .exit.t = 0.5}};

//     SpanList result;
//     ASSERT_TRUE(subtractSpanLists(&original, &subtracted, &result));

//     ASSERT_TRUE(result.n == 1);
//     EXPECT_TRUE(result.intervals[0].entry.t == original.intervals[0].entry.t);
//     EXPECT_TRUE(result.intervals[0].exit.t == original.intervals[0].exit.t);
// }

// /* Result: expect false */
// TEST(SpanList, TestCompleteOverlapSubtraction)
// {
//     SpanList original = {.n = 1, .intervals[0] = {.entry.t = 1, .exit.t = 5}};
//     SpanList subtracted = {.n = 1, .intervals[0] = {.entry.t = 1, .exit.t = 5}};

//     SpanList result;
//     EXPECT_FALSE(subtractSpanLists(&original, &subtracted, &result));
// }


// /* Result: expect false */
// TEST(SpanList, TestNullMinusNull)
// {
//     SpanList result;
//     EXPECT_FALSE(subtractSpanLists(nullptr, nullptr, &result));
// }


// /* Result: expect to have the original subset */
// TEST(SpanList, TestNonNullMinusNull)
// {
//     SpanList original = {.n = 1, .intervals[0] = {.entry.t = 1, .exit.t = 5}};

//     SpanList result;
//     ASSERT_TRUE(subtractSpanLists(&original, nullptr, &result));

//     ASSERT_TRUE(result.n == 1);
//     EXPECT_TRUE(result.intervals[0].entry.t == original.intervals[0].entry.t);
//     EXPECT_TRUE(result.intervals[0].exit.t == original.intervals[0].exit.t);
// }


// /* Result: expect false */
// TEST(SpanList, TestNullMinusNonNull)
// {
//     SpanList subtracted = {.n = 1, .intervals[0] = {.entry.t = 2, .exit.t = 3}};

//     SpanList result;
//     EXPECT_FALSE(subtractSpanLists(nullptr, &subtracted, &result));
// }