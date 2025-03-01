/**
 * @file CSGInterval.c
 * @author Edward Palmer
 * @date 2025-02-27
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "CSGInterval.h"

bool insideInterval(const CSGInterval *interval, double t)
{
    if (!interval) return false;

    return (t >= interval->tEnter && t <= interval->tExit);
}


bool intervalsOverlap(const CSGInterval *interval1, const CSGInterval *interval2)
{
    if (!interval1 || !interval2) return false;

    bool interval2InsideInterval1 =
        (insideInterval(interval1, interval2->tEnter) || insideInterval(interval1, interval2->tExit));
    bool interval1InsideInterval2 =
        (insideInterval(interval2, interval1->tEnter) || insideInterval(interval2, interval1->tExit));

    return (interval2InsideInterval1 || interval1InsideInterval2);
}


bool isSubInterval(const CSGInterval *interval, const CSGInterval *subinterval)
{
    if (!interval || !subinterval) return false;

    return (insideInterval(interval, subinterval->tEnter) && insideInterval(interval, subinterval->tExit));
}


int subtractIntervals(const CSGInterval *originalInterval, const CSGInterval *intervalToSubtract, CSGInterval result[2])
{
    if (!originalInterval || !intervalToSubtract || !result) return 0;

    if (!intervalsOverlap(originalInterval, intervalToSubtract))
    {
        return 0;
    }

    if (isSubInterval(intervalToSubtract, originalInterval))
    {
        // Original interval completely inside interval we're subtracting --> nothing left.
        return 0;
    }

    if (isSubInterval(originalInterval, intervalToSubtract))
    {
        // Interval we're subtracting is inside original interval --> 2 outputs.
        result[0].tEnter = originalInterval->tEnter;
        result[0].tExit = intervalToSubtract->tEnter;

        result[1].tEnter = intervalToSubtract->tExit;
        result[1].tExit = originalInterval->tExit;

        return 2;
    }

    // Single output:
    if (insideInterval(originalInterval, intervalToSubtract->tEnter))
    {
        result[0].tEnter = originalInterval->tEnter;
        result[0].tExit = intervalToSubtract->tEnter;
    }
    else
    {
        result[0].tEnter = intervalToSubtract->tExit;
        result[0].tExit = originalInterval->tExit;
    }

    return 1;
}