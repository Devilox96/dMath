#include <benchmark/benchmark.h>
#include "../Vector.h"
#include <random>

namespace benchmark {
using dmath::Vector;

constexpr std::size_t NUM = 20'000;
constexpr std::size_t REP = 5;

template <class T, std::size_t N>
class VectorFixture : public ::benchmark::Fixture {
public:
    static_assert(std::is_floating_point_v<T>);
    void SetUp(const ::benchmark::State& st) {
        mGenerator.seed();
        mVector = mVector.generate(mGen);
    }

    std::default_random_engine mGenerator;
    std::uniform_real_distribution<T> mDistribution = std::uniform_real_distribution<T>(-10.0, 10.0);
    std::function<T(void)> mGen = [&](){ return mDistribution(mGenerator); };

    Vector<T, N> mVector;
};

using VectorDobule100 = VectorFixture<double, 100>;

BENCHMARK_DEFINE_F(VectorDobule100, plusLambda)(benchmark::State& state) {
    for (auto _ : state) {
        mVector = plusLambda(mGen(), mVector);
    }
}
BENCHMARK_REGISTER_F(VectorDobule100, plusLambda)->Iterations(NUM)->Repetitions(REP);

BENCHMARK_DEFINE_F(VectorDobule100, plusFunctor)(benchmark::State& state) {
    for (auto _ : state) {
        mVector = plusFunctor(mGen(), mVector);
    }
}
BENCHMARK_REGISTER_F(VectorDobule100, plusFunctor)->Iterations(NUM)->Repetitions(REP);

BENCHMARK_DEFINE_F(VectorDobule100, plusForSub)(benchmark::State& state) {
    for (auto _ : state) {
        mVector = plusForSub(mGen(), mVector);
    }
}
BENCHMARK_REGISTER_F(VectorDobule100, plusForSub)->Iterations(NUM)->Repetitions(REP);

BENCHMARK_DEFINE_F(VectorDobule100, plusForPtr)(benchmark::State& state) {
    for (auto _ : state) {
        mVector = plusForPtr(mGen(), mVector);
    }
}
BENCHMARK_REGISTER_F(VectorDobule100, plusForPtr)->Iterations(NUM)->Repetitions(REP);

} // namespace benchmark
BENCHMARK_MAIN();
