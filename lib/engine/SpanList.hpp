/**
 * @file SpanList.h
 * @author Edward Palmer
 * @date 2025-02-23
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <vector>

extern "C"
{
#include "engine/Span.h"
}

/**
 * Stores all intersections.
 */
// typedef struct SpanList_t
// {
//     int n;
//     SpanRec intervals[20];
// } SpanList;

using SpanList = std::vector<SpanRec>;

/**
 * @brief Computes the list of intervals after subtracting a list from an original list. Returns false if there are
 * no intervals after the subtraction.
 */
bool subtractSpanLists(const SpanList &original, const SpanList &subtractor, SpanList &result);
