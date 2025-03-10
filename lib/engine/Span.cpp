/**
 * @file Span.cpp
 * @author Edward Palmer
 * @date 2025-03-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Span.hpp"
#include <iostream>

Span::Span(double tentry, double texit)
{
    entry.t = tentry;
    exit.t = texit;
}


bool Span::insideInterval(double t, double tolerance) const
{
    return (t >= (entry.t + tolerance) && t <= (exit.t - tolerance));
}


bool Span::intervalsOverlap(const Span &other) const
{
    bool interval2InsideInterval1 = (insideInterval(other.entry.t) || insideInterval(other.exit.t));
    bool interval1InsideInterval2 = (other.insideInterval(entry.t) || other.insideInterval(exit.t));

    return (interval2InsideInterval1 || interval1InsideInterval2);
}


bool Span::isSubInterval(const Span &other) const
{
    return (insideInterval(other.entry.t) && insideInterval(other.exit.t));
}


int Span::subtractIntervals(const Span &other, std::array<Span, 2> &result) const
{
    const double kTolerance = 1e-12;

    if (!intervalsOverlap(other))
    {
        return (-1);
    }

    if (other.isSubInterval(*this))
    {
        // Original interval completely inside interval we're subtracting --> nothing left.
        return 0;
    }

    if (isSubInterval(other))
    {
        // TODO: - should probably check for precision problems in here as well.

        // Interval we're subtracting is inside original interval --> 2 outputs.
        result[0].entry = entry;
        result[0].exit = other.entry;

        result[1].entry = other.exit;
        result[1].exit = exit;

        return 2;
    }

    // Single output:
    if (insideInterval(other.entry.t, kTolerance))
    {
        result[0].entry = entry;
        result[0].exit = other.entry;
        return 1;
    }
    else if (insideInterval(other.exit.t, kTolerance))
    {
        result[0].entry = other.exit;
        result[0].exit = exit;
        return 1;
    }
    else
    {
        // This must be the case where the intervals very very slightly overlap but we'll get all sorts of weird
        // artifacts so we return 0. This results in these results being ignored when we are subtracting multiple spans.
        return 0;
    }
}


int Span::subtractSpanLists(const SpanList &origList, const SpanList &otherList, SpanList &result)
{
    if (origList.empty()) // No result list or nothing to subtract from --> we have nothing.
    {
        return 0;
    }

    if (otherList.empty()) // Nothing to subtract --> copy list1.
    {
        std::copy(origList.begin(), origList.end(), std::back_inserter(result));
        return result.size();
    }

    // We have two non-empty lists. We need to subtract them:
    std::vector<Span> stack;

    // Copy intervals onto stack.
    std::copy(origList.begin(), origList.end(), std::back_inserter(stack));

    // Keep looping over until we have no more splits.
    for (int i = 0; i < stack.size(); ++i)
    {
        std::array<Span, 2> output;
        bool isStale{false};

        // Iterate over subtractor to find sub-stacks.
        for (auto &otherSpan : otherList)
        {
            int n = stack[i].subtractIntervals(otherSpan, output);

            if (n == (-1)) // No overlap.
            {
                continue;
            }

            if (n == 1)
            {
                stack[i] = output[0];
            }
            else if (n == 2)
            {
                stack[i] = output[0];
                stack.push_back(output[1]);
            }
            else if (n == 0)
            {
                isStale = true;
                break;
            }
        }

        if (!isStale)
        {
            result.push_back(stack[i]);
        }
    }

    return (result.size());
}