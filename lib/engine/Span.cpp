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


bool Span::completeOverlap(const Span &other, double tolerance) const
{
    return (fabs(entry.t - other.entry.t) < tolerance && fabs(exit.t - other.exit.t) < tolerance);
}


bool Span::intervalsOverlap(const Span &other) const
{
    if (completeOverlap(other))
    {
        return true;
    }

    bool interval2InsideInterval1 = (insideInterval(other.entry.t) || insideInterval(other.exit.t));
    bool interval1InsideInterval2 = (other.insideInterval(entry.t) || other.insideInterval(exit.t));

    return (interval2InsideInterval1 || interval1InsideInterval2);
}


bool Span::isSubInterval(const Span &other) const
{
    return (insideInterval(other.entry.t, 0) && insideInterval(other.exit.t, 0));
}


int Span::subtractIntervals(const Span &rhs, std::array<Span, 2> &result) const
{
    // Check for complete overlap and return 0 to remove span.
    if (completeOverlap(rhs))
    {
        return 0;
    }

    // NB: returning (-1) so lhs span is not removed.
    if (!intervalsOverlap(rhs))
    {
        return (-1);
    }

    int nReturnValues = 0;

    /**
     * Original: |..............|
     * RHS:              |...........|
     * Result:   |.......|
     *
     * If RHS entry is very close to original entry then we have nothing left --> don't add.
     */
    if (insideInterval(rhs.entry.t))
    {
        result[nReturnValues].entry = entry;
        result[nReturnValues].exit = rhs.entry;

        nReturnValues++;
    }

    /**
     * Original:      |..............|
     * RHS:      |...............|
     * Result:                   |...|
     *
     * If RHS exit is very close to original exit then we have nothing left --> don't add.
     */
    if (insideInterval(rhs.exit.t))
    {
        result[nReturnValues].entry = rhs.exit;
        result[nReturnValues].exit = exit;

        nReturnValues++;
    }

    return nReturnValues;
}


std::vector<Span> Span::recursiveSpanSubtractor(const Span &lhs, const SpanList::iterator subtractFirst,
                                                const SpanList::iterator subtractLast)
{
    std::array<Span, 2> output;

    for (SpanList::iterator iter = subtractFirst; iter != subtractLast; ++iter)
    {
        int n = lhs.subtractIntervals(*iter, output);

        switch (n)
        {
            case 0:
                return {}; // Empty vector (complete overlap).
            case 1:
                return recursiveSpanSubtractor(output[0], iter + 1, subtractLast);
            case 2:
            {
                SpanList leftResult = recursiveSpanSubtractor(output[0], iter + 1, subtractLast);
                SpanList rightResult = recursiveSpanSubtractor(output[1], iter + 1, subtractLast);

                leftResult.insert(leftResult.end(), rightResult.begin(), rightResult.end());
                return leftResult;
            }
            case (-1): // Continue. No overlap.
            default:
                break;
        }
    }

    // No overlaps.
    return {lhs};
}


int Span::subtractSpanLists(const SpanList &origList, SpanList &otherList, SpanList &result)
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

    result.clear();

    // Keep looping over until we have no more splits.
    for (auto &lhsSpan : origList)
    {
        auto subSpans = recursiveSpanSubtractor(lhsSpan, otherList.begin(), otherList.end());

        if (subSpans.size())
        {
            result.insert(result.end(), subSpans.begin(), subSpans.end());
        }
    }

    return (result.size());
}