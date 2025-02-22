/**
 * @file BenchmarkVector3.cpp
 * @author Edward Palmer
 * @date 2025-02-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <benchmark/benchmark.h>

extern "C"
{
#include "utility/Vector3.h"
}

static void BenchmarkRandomUnitVector(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)randomUnitVector();
    }
}

// TODO: - add benchmarks using data YAML files that we will store somewhere.

BENCHMARK(BenchmarkRandomUnitVector);
