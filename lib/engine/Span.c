/**
 * @file Span.c
 * @author Edward Palmer
 * @date 2025-03-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Span.h"
#include <stdio.h>

bool insideInterval(const SpanRec *interval, double t)
{
    if (!interval) return false;

    return (t >= interval->entry.t && t <= interval->exit.t);
}


bool intervalsOverlap(const SpanRec *interval1, const SpanRec *interval2)
{
    if (!interval1 || !interval2) return false;

    bool interval2InsideInterval1 =
        (insideInterval(interval1, interval2->entry.t) || insideInterval(interval1, interval2->exit.t));
    bool interval1InsideInterval2 =
        (insideInterval(interval2, interval1->entry.t) || insideInterval(interval2, interval1->exit.t));

    return (interval2InsideInterval1 || interval1InsideInterval2);
}


bool isSubInterval(const SpanRec *interval, const SpanRec *subinterval)
{
    if (!interval || !subinterval) return false;

    return (insideInterval(interval, subinterval->entry.t) && insideInterval(interval, subinterval->exit.t));
}


int subtractIntervals(const SpanRec *originalInterval, const SpanRec *intervalToSubtract, SpanRec result[2])
{
    if (!originalInterval || !intervalToSubtract || !result) return 0;

    // Small offset value to avoid ugly issues:
    const static double kDelta = 1e-6;

    if (!intervalsOverlap(originalInterval, intervalToSubtract))
    {
        return (-1);
    }

    if (isSubInterval(intervalToSubtract, originalInterval))
    {
        // Original interval completely inside interval we're subtracting --> nothing left.
        return 0;
    }

    if (isSubInterval(originalInterval, intervalToSubtract))
    {
        // TODO: - should probably check for precision problems in here as well.

        // Interval we're subtracting is inside original interval --> 2 outputs.
        result[0].entry = originalInterval->entry;

        result[0].exit = intervalToSubtract->entry;
        // if (intervalToSubtract->entry.frontFace)
        {
            // result[0].exit.normal = flipVector(intervalToSubtract->entry.normal);
        }

        result[1].entry = intervalToSubtract->exit;
        // if (intervalToSubtract->exit.frontFace)
        {
            // result[1].entry.normal = flipVector(intervalToSubtract->exit.normal);
        }

        result[1].exit = originalInterval->exit;

        return 2;
    }

    // Single output:
    if (insideInterval(originalInterval, intervalToSubtract->entry.t))
    {
        if (fabs(originalInterval->exit.t - intervalToSubtract->entry.t) < kDelta)
        {
            return 0; // Avoid precision problems if very close.
        }

        if (intervalToSubtract->entry.t <= 0.0)
        {
            return 0; // New span is all before camera so ignore.
        }

        result[0].entry = originalInterval->entry;

        result[0].exit = intervalToSubtract->entry;
        // if (intervalToSubtract->entry.frontFace)
        {
            // result[0].entry.normal = flipVector(intervalToSubtract->entry.normal);
        }

        return 1;
    }
    else if (insideInterval(originalInterval, intervalToSubtract->exit.t)) // TODO: - Unnecessary check
    {
        if (fabs(originalInterval->entry.t - intervalToSubtract->exit.t) < kDelta)
        {
            return 0; // Avoid precision problems if very close.
        }

        result[0].entry = intervalToSubtract->exit;
        // if (intervalToSubtract->exit.frontFace)
        {
            // result[0].entry.normal = flipVector(intervalToSubtract->exit.normal);
        }

        result[0].exit = originalInterval->exit;

        return 1;
    }

    return 0;
}