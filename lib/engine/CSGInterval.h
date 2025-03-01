/**
 * @file CSGInterval.h
 * @author Edward Palmer
 * @date 2025-02-27
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <stdbool.h>

/**
 * Store the enter and exit times of a ray with a primitive.
 */
typedef struct CSGInterval_t
{
    double tEnter;
    double tExit;
} CSGInterval;


/**
 * @brief Returns true if time period is in range [tEnter, tExit].
 * @param interval Pointer to the interval to test.
 * @param t The time to check.
 * @return Returns true if inside interval.
 */
bool insideInterval(const CSGInterval *interval, double t);


/**
 * @brief Returns true if intervals overlap.
 * @param interval1
 * @param interval2
 */
bool intervalsOverlap(const CSGInterval *interval1, const CSGInterval *interval2);


/**
 * @brief Returns true if subinterval completely contained inside the interval.
 */
bool isSubInterval(const CSGInterval *interval, const CSGInterval *subinterval);


/**
 * @brief interval1 - interval2.
 * @param interval1
 * @param interval2
 * @param result An array containing the output interval(s).
 * @returns Returns the number of output intervals (0, 1 or 2).
 */
int subtractIntervals(const CSGInterval *interval1, const CSGInterval *interval2, CSGInterval result[2]);
