/**
 * @file Span.cpp
 * @author Edward Palmer
 * @date 2025-03-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Span.hpp"

bool Span::insideInterval(double t) const
{
    return (t >= entry.t && t <= exit.t);
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
    // Small offset value to avoid ugly issues:
    const static double kDelta = 1e-6;

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
    if (insideInterval(other.entry.t))
    {
        if (fabs(exit.t - other.entry.t) < kDelta)
        {
            return 0; // Avoid precision problems if very close.
        }

        if (other.entry.t <= 0.0)
        {
            return 0; // New span is all before camera so ignore.
        }

        result[0].entry = entry;
        result[0].exit = other.entry;

        return 1;
    }
    else if (insideInterval(other.exit.t))
    {
        if (fabs(entry.t - other.exit.t) < kDelta)
        {
            return 0; // Avoid precision problems if very close.
        }

        result[0].entry = other.exit;
        result[0].exit = exit;

        return 1;
    }

    return 0;
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
        Span &span = stack[i];

        std::array<Span, 2> output;
        bool isStale{false};

        // Iterate over subtractor to find sub-stacks.
        for (auto &subtractSpan : otherList)
        {
            int n = span.subtractIntervals(subtractSpan, output);

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
            result.push_back(span);
        }
    }

    return (result.size());
}