/**
 * @file Span.h
 * @author Edward Palmer
 * @date 2025-03-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include "engine/HitRec.h"
#include <stdbool.h>

/**
 * Stores the hit on entry and exit of a primitive.
 */
typedef struct
{
    HitRec entry;
    HitRec exit;
} SpanRec;


/**
 * @brief Returns true if time period is in range [tEnter, tExit].
 * @param interval Pointer to the interval to test.
 * @param t The time to check.
 * @return Returns true if inside interval.
 */
bool insideInterval(const SpanRec *interval, double t);


/**
 * @brief Returns true if intervals overlap.
 * @param interval1
 * @param interval2
 */
bool intervalsOverlap(const SpanRec *interval1, const SpanRec *interval2);


/**
 * @brief Returns true if subinterval completely contained inside the interval.
 */
bool isSubInterval(const SpanRec *interval, const SpanRec *subinterval);


/**
 * @brief interval1 - interval2.
 * @param interval1
 * @param interval2
 * @param result An array containing the output interval(s).
 * @returns Returns the number of output intervals (-1, 0, 1 or 2).
 *
 * -1 --> intervals do not overlap
 * 0, 1, 2 --> number of intervals after subtraction.
 */
int subtractIntervals(const SpanRec *interval1, const SpanRec *interval2, SpanRec result[2]);