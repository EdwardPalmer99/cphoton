/**
 * @file SpanList.c
 * @author Edward Palmer
 * @date 2025-02-23
 *
 * @copyright Copyright (c) 2025
 *
 */


extern "C"
{
#include "logger/Logger.h"
}

#include "engine/SpanList.hpp"
#include <algorithm>

/**
 * Returns the intersection times list after subtracting original from subtractor.
 */
bool subtractSpanLists(const SpanList &original, const SpanList &subtractor, SpanList &result)
{
    if (original.empty()) // No result list or nothing to subtract from --> we have nothing.
    {
        return false;
    }

    if (subtractor.empty()) // Nothing to subtract --> copy list1.
    {
        std::copy(original.begin(), original.end(), std::back_inserter(result));
        return true;
    }

    // We have two non-empty lists. We need to subtract them:
    std::vector<SpanRec> stack;

    // Copy intervals onto stack.
    std::copy(original.begin(), original.end(), std::back_inserter(stack));

    // Keep looping over until we have no more splits.
    for (int i = 0; i < stack.size(); ++i)
    {
        SpanRec &span = stack[i];

        SpanRec output[2];
        bool isStale{false};

        // Iterate over subtractor to find sub-stacks.
        for (auto &subtractSpan : subtractor)
        {
            int n = subtractIntervals(&span, &subtractSpan, output);

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
