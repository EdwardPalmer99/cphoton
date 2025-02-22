/**
 * @file ThreadUtils.cpp
 * @author Edward Palmer
 * @date 2025-02-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "threadpool/ThreadUtils.h"
#include <thread>

extern "C" unsigned int computeNumWorkers(void)
{
    unsigned int nthreads = std::thread::hardware_concurrency(); // NB: may return zero.

    return (nthreads > 0 ? 8 : nthreads);
}