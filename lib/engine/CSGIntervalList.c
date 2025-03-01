/**
 * @file CSGIntersectionTime.c
 * @author Edward Palmer
 * @date 2025-02-23
 *
 * @copyright Copyright (c) 2025
 *
 */


#include "CSGIntervalList.h"
#include "logger/Logger.h"
#include <stdlib.h>
#include <string.h>

/**
 * Returns the intersection times list after subtracting original from subtractor.
 */
bool subtractIntervalLists(const CSGIntervalList *original, const CSGIntervalList *subtractor, CSGIntervalList *result)
{
    if (!result || !(original && original->n > 0)) // No result list or nothing to subtract from --> we have nothing.
    {
        return false;
    }

    if (!(subtractor && subtractor->n > 0)) // Nothing to subtract --> copoy list1.
    {
        result->n = original->n;
        memcpy(result->intervals, original->intervals, sizeof(CSGInterval) * original->n);
        return true;
    }

    // We have two non-empty lists. We need to subtract them:
    int stackSize = original->n;

    CSGInterval stack1[20];
    bool isStale[20] = {0};

    // Copy intervals onto stack.
    memcpy(stack1, original->intervals, sizeof(CSGInterval) * original->n);

    // Keep looping over until we have no more splits.
    for (int iStackPtr = 0, didHaveSplit = false; iStackPtr < stackSize; ++iStackPtr)
    {
        if (isStale[iStackPtr])
        {
            continue;
        }

        CSGInterval output[2];

        // Iterate over subtractor to find sub-stacks.
        for (int i = 0; i < subtractor->n; ++i)
        {
            int n = subtractIntervals(&stack1[iStackPtr], &subtractor->intervals[i], output);

            if (n != 0) // We had 1 or 2 intersections --> mark the item as stale.
            {
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
