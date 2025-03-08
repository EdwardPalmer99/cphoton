/**
 * @file Span.hpp
 * @author Edward Palmer
 * @date 2025-03-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/HitRec.h"
#include <array>
#include <vector>

/**
 * Stores the hit on entry and exit of a primitive.
 */
struct SpanRec
{
    HitRec entry;
    HitRec exit;

    /** Returns true if time t is inside span */
    bool insideInterval(double t) const;

    /** Returns true if other span overlaps */
    bool intervalsOverlap(const SpanRec &other) const;

    /** Returns true if other span is subinterval (contained completely with span) */
    bool isSubInterval(const SpanRec &other) const;

    /**
     * @brief subtract other from this interval.
     * @returns Returns the number of output intervals (-1, 0, 1 or 2).
     * -1 --> intervals do not overlap
     * 0, 1, 2 --> number of intervals after subtraction.
     */
    int subtractIntervals(const SpanRec &other, std::array<SpanRec, 2> &result) const;

    using SpanList = std::vector<SpanRec>;

    /** Subtracts otherList spans from origList */
    static int subtractSpanLists(const SpanList &origList, const SpanList &otherList, SpanList &result);
};