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
#include <stdlib.h>
#include <string.h>

/**
 * Returns the intersection times list after subtracting original from subtractor.
 */
bool subtractSpanLists(const SpanList *original, const SpanList *subtractor, SpanList *result)
{
    if (!result ||
        !(original && original->size() > 0)) // No result list or nothing to subtract from --> we have nothing.
    {
        return false;
    }

    if (!(subtractor && subtractor->size() > 0)) // Nothing to subtract --> copy list1.
    {
        result->clear();
        std::copy(original->begin(), original->end(), std::back_inserter(*result));
        return true;
    }

    // We have two non-empty lists. We need to subtract them:
    bool isStale[20] = {0};
    std::vector<SpanRec> stack1;

    // Copy intervals onto stack.
    std::copy(original->begin(), original->end(), std::back_inserter(stack1));

    result->clear();

    // Keep looping over until we have no more splits.
    for (int iStackPtr = 0; iStackPtr < stack1.size(); ++iStackPtr)
    {
        if (isStale[iStackPtr])
        {
            continue;
        }

        SpanRec output[2];

        // Iterate over subtractor to find sub-stacks.
        for (int i = 0; i < subtractor->size(); ++i)
        {
            int n = subtractIntervals(&stack1[iStackPtr], &((*subtractor)[i]), output);

            if (n == (-1)) // No overlap.
            {
                continue;
            }

            if (n == 1)
            {
                stack1[iStackPtr] = output[0];
            }
            else if (n == 2)
            {
                stack1[iStackPtr] = output[0];
                stack1.push_back(output[1]);
            }
            else if (n == 0)
            {
                isStale[iStackPtr] = true;
                break;
            }
        }
    }

    // Copy cleaned list:
    for (int i = 0; i < stack1.size(); ++i)
    {
        if (!isStale[i])
        {
            result->push_back(stack1[i]);
        }
    }

    return (result->size() > 0);
}
