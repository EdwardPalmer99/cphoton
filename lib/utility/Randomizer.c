/**
 * @file Randomizer.c
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Randomizer.h"
#include <stdlib.h>

/// Returns a double in the range [0, 1).
double randomDouble(void)
{
    return (double)rand() / (RAND_MAX + 1.0);
}


/// Returns a double in the range [min, max).
double randomDoubleRange(double min, double max)
{
    return min + randomDouble() * (max - min);
}