/**
 * @file Span.cpp
 * @author Edward Palmer
 * @date 2025-03-01
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "Span.hpp"
#include "utility/Vector3.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>


Span::Span(double tentry, double texit)
{
    entry.t = tentry;
    exit.t = texit;
}

Span::Span(Hit entry_, Hit exit_) : entry(std::move(entry_)), exit(std::move(exit_))
{
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
    if (insideInterval(rhs.entry.t, 0.1))
    {
        result[nReturnValues].entry = entry;

        result[nReturnValues].exit = rhs.entry;
        result[nReturnValues].exit.frontFace = false;
        result[nReturnValues].exit.normal = -rhs.entry.normal;

        nReturnValues++;
    }

    /**
     * Original:      |..............|
     * RHS:      |...............|
     * Result:                   |...|
     *
     * If RHS exit is very close to original exit then we have nothing left --> don't add.
     */
    if (insideInterval(rhs.exit.t, 0.1))
    {
        result[nReturnValues].entry = rhs.exit;
        result[nReturnValues].entry.frontFace = false;
        result[nReturnValues].entry.normal = -rhs.exit.normal;

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


int Span::differenceSpanLists(const SpanList &origList, SpanList &otherList, SpanList &result)
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

    // Sort by increasing t-entry.
    std::sort(result.begin(), result.end(),
              [](const Span &left, const Span &right) { return left.entry.t < right.entry.t; });

    return (result.size());
}


int Span::unionSpanLists(const SpanList &origList, const SpanList &otherList, SpanList &result)
{
    // Edge cases.
    if (origList.empty())
    {
        std::copy(otherList.begin(), otherList.end(), std::back_inserter(result));
        return result.size();
    }
    else if (otherList.empty())
    {
        std::copy(origList.begin(), origList.end(), std::back_inserter(result));
        return result.size();
    }

    std::list<Span> spans;
    std::copy(origList.begin(), origList.end(), std::back_inserter(spans));
    std::copy(otherList.begin(), otherList.end(), std::back_inserter(spans));

    bool eraseIter = false;

    for (auto iter = spans.begin(); iter != spans.end();)
    {
        eraseIter = false;

        for (auto iter2 = spans.begin(); iter2 != spans.end();)
        {
            if (iter2 != iter && (*iter).intervalsOverlap(*iter2))
            {
                Hit &minEntry = (*iter).entry.t < (*iter2).entry.t ? (*iter).entry : (*iter2).entry;
                Hit &maxExit = (*iter).exit.t > (*iter2).exit.t ? (*iter).exit : (*iter2).exit;

                spans.push_back(Span(minEntry, maxExit));
                eraseIter = true;
                spans.erase(iter2++);
                break;
            }
            else
            {
                iter2++;
            }
        }

        if (eraseIter)
        {
            spans.erase(iter++);
        }
        else
        {
            iter++;
        }
    }

    result.clear();
    std::copy(spans.begin(), spans.end(), std::back_inserter(result));

    std::sort(result.begin(), result.end(),
              [](const Span &left, const Span &right) { return left.entry.t < right.entry.t; });

    return result.size();
}


int Span::intersectionSpanLists(const SpanList &origList, const SpanList &otherList, SpanList &result)
{
    // Edge cases.
    if (origList.empty() || otherList.empty())
    {
        result.clear();
        return 0;
    }

    result.clear();

    for (auto &otherSpan : otherList)
    {
        for (auto &origSpan : origList)
        {
            if (otherSpan.intervalsOverlap(origSpan))
            {
                // Find the exact overlap:
                const Hit &entryIntersection = otherSpan.entry.t > origSpan.entry.t ? otherSpan.entry : origSpan.entry;
                const Hit &exitIntersection = otherSpan.exit.t < origSpan.exit.t ? otherSpan.exit : origSpan.exit;

                // Ignore any spans we create where both tmin, tmax are less than zero.
                if (exitIntersection.t > 0.0)
                {
                    result.push_back(Span(entryIntersection, exitIntersection));
                }
            }
        }
    }

    std::sort(result.begin(), result.end(),
              [](const Span &left, const Span &right) { return left.entry.t < right.entry.t; });

    return result.size();
}
