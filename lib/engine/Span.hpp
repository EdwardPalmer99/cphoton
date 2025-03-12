/**
 * @file Span.hpp
 * @author Edward Palmer
 * @date 2025-03-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <array>
#include <vector>

extern "C"
{
#include "engine/HitRec.h"
}

/**
 * Stores the hit on entry and exit of a primitive.
 */
struct Span
{
    HitRec entry;
    HitRec exit;

    // Construct a span in range [tentry, texit]. Useful for testing
    Span() = default;
    Span(double tentry, double texit);
    Span(HitRec entry, HitRec exit);

    /** Returns true if time t is inside span */
    bool insideInterval(double t, double tolerance = 1e-6) const;

    /** Returns true on complete overlap */
    bool completeOverlap(const Span &other, double tolerance = 1e-6) const;

    /** Returns true if other span overlaps */
    bool intervalsOverlap(const Span &other) const;

    /** Returns true if other span is subinterval (contained completely with span) */
    bool isSubInterval(const Span &other) const;

    /**
     * @brief subtract other from this interval.
     * @returns Returns the number of output intervals (-1, 0, 1 or 2).
     * -1 --> intervals do not overlap
     * 0, 1, 2 --> number of intervals after subtraction.
     */
    int subtractIntervals(const Span &other, std::array<Span, 2> &result) const;

    using SpanList = std::vector<Span>;

    /** Subtracts otherList spans from origList */
    static int differenceSpanLists(const SpanList &origList, SpanList &otherList, SpanList &result);

    /**
     * Union operation on two span lists
     * Assumptions:
     * - No spans overlap within origList (and otherList)
     *
     */
    static int unionSpanLists(const SpanList &origList, const SpanList &otherList, SpanList &result);

    static int intersectionSpanLists(const SpanList &origList, const SpanList &otherList, SpanList &result);

    /** Recursive method which returns all subspans */
    static SpanList recursiveSpanSubtractor(const Span &lhs, const SpanList::iterator subtractFirst,
                                            const SpanList::iterator subtractLast);
};