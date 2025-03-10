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


int Span::subtractIntervals(const Span &rhs, std::array<Span, 2> &result) const
{
    // Small offset value to avoid ugly issues:
    const static double kDelta = 1e-6;

    // NB: returning (-1) so lhs span is not removed.
    if (!intervalsOverlap(rhs))
    {
        return (-1);
    }

    int nReturnValues = 0;

    // NB: if output spans are very small then ignore.
    if (insideInterval(rhs.entry.t) && (fabs(exit.t - rhs.entry.t) > kDelta))
    {
        result[nReturnValues].entry = entry;
        result[nReturnValues].exit = rhs.entry;

        nReturnValues++;
    }

    if (insideInterval(rhs.exit.t) && (fabs(entry.t - rhs.exit.t) > kDelta))
    {
        result[nReturnValues].entry = rhs.exit;
        result[nReturnValues].exit = exit;

        nReturnValues++;
    }

    return nReturnValues;
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