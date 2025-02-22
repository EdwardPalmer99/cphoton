/**
 * @file demo.cpp
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

extern "C"
{
#include "utility/Randomizer.h"
}

#include <gtest/gtest.h>

TEST(Randomizer, TestRandomDoubleRange)
{
    double min = 1.0, max = 2.0;

    double value = randomDoubleRange(min, max);
    EXPECT_TRUE(value >= min && value < max);
}