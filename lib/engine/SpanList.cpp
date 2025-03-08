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
    if (!result || !(original && original->n > 0)) // No result list or nothing to subtract from --> we have nothing.
    {
        return false;
    }

    if (!(subtractor && subtractor->n > 0)) // Nothing to subtract --> copy list1.
    {
        result->n = original->n;
        memcpy(result->intervals, original->intervals, sizeof(SpanRec) * original->n);
        return true;
    }

    // We have two non-empty lists. We need to subtract them:
    int stackSize = original->n;

    SpanRec stack1[20];
    bool isStale[20] = {0};

    // Copy intervals onto stack.
    memcpy(stack1, original->intervals, sizeof(SpanRec) * original->n);

    // Keep looping over until we have no more splits.
    for (int iStackPtr = 0; iStackPtr < stackSize; ++iStackPtr)
    {
        if (isStale[iStackPtr])
        {
            continue;
        }

        SpanRec output[2];

        // Iterate over subtractor to find sub-stacks.
        for (int i = 0; i < subtractor->n; ++i)
        {
            int n = subtractIntervals(&stack1[iStackPtr], &subtractor->intervals[i], output);

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
                stack1[stackSize++] = output[1];
            }
            else if (n == 0)
            {
                isStale[iStackPtr] = true;
                break;
            }
        }
    }

    // Copy cleaned list:
    int iResult = 0;

    for (int i = 0; i < stackSize; ++i)
    {
        if (!isStale[i])
        {
            result->intervals[iResult++] = stack1[i];
        }
    }

    result->n = iResult;

    return (result->n > 0);
}
