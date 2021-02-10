#include <benchmark/benchmark.h>
#include "../dVector.h"

static void BM_dVectorMultiply(benchmark::State& state) {
    dVector<double, 10000> d1(15);
    for (auto _ : state) {
        dVector<double, 10000> d1(15);
        dVector<double, 10000> d2(15);
        auto d = d1 * d2;
    }
}
BENCHMARK(BM_dVectorMultiply);
// Run the benchmark

static void BM_dVectorDot(benchmark::State& state) {
    dVector<double, 10000> d1(15);
    for (auto _ : state) {
        dVector<double, 10000> d1(15);
        dVector<double, 10000> d2(15);
        auto d = dot(d1, d2);
    }
}
BENCHMARK(BM_dVectorMultiply);

BENCHMARK_MAIN();
