/**
 * @file CSGIntersection.h
 * @author Edward Palmer
 * @date 2025-02-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "CSGInterval.h"
#include <stdbool.h>

/**
 * Stores all intersections.
 */
typedef struct CSGIntervalList_t
{
    int n;
    CSGInterval intervals[20];
} CSGIntervalList;

/**
 * @brief Computes the list of intervals after subtracting a list from an original list. Returns false if there are no
 * intervals after the subtraction.
 */
bool subtractIntervalLists(const CSGIntervalList *original, const CSGIntervalList *subtractor, CSGIntervalList *result);
