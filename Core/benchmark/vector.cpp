#include <benchmark/benchmark.h>
#include "../Vector.h"
#include <random>


constexpr std::size_t NUM = 20'000'000;
constexpr std::size_t REP = 10;

template <class T, std::size_t N>
class VectorFixture : public ::benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& st) {
        for(int i = 0; i < vector_.size(); ++i) {
            vector_[i] = distr_(generator_);
        }
    }
    std::default_random_engine generator_;
    std::uniform_real_distribution<double> distr_ = std::uniform_real_distribution<double>(-10.0, 10.0);
    dmath::Vector<T, N> vector_;
};

using VectorDobule100 = VectorFixture<double, 100>;

BENCHMARK_DEFINE_F(VectorDobule100, plusLambda)(benchmark::State& state) {
    for (auto _ : state) {
        vector_ = plusLambda(distr_(generator_), vector_);
    }
}
BENCHMARK_REGISTER_F(VectorDobule100, plusLambda)->Iterations(NUM)->Repetitions(REP);

BENCHMARK_DEFINE_F(VectorDobule100, plusFunctor)(benchmark::State& state) {
    for (auto _ : state) {
        vector_ = plusFunctor(distr_(generator_), vector_);
    }
}
BENCHMARK_REGISTER_F(VectorDobule100, plusFunctor)->Iterations(NUM)->Repetitions(REP);

BENCHMARK_DEFINE_F(VectorDobule100, plusForSub)(benchmark::State& state) {
    for (auto _ : state) {
        vector_ = plusForSub(distr_(generator_), vector_);
    }
}
BENCHMARK_REGISTER_F(VectorDobule100, plusForSub)->Iterations(NUM)->Repetitions(REP);

BENCHMARK_DEFINE_F(VectorDobule100, plusForPtr)(benchmark::State& state) {
    for (auto _ : state) {
        vector_ = plusForPtr(distr_(generator_), vector_);
    }
}
BENCHMARK_REGISTER_F(VectorDobule100, plusForPtr)->Iterations(NUM)->Repetitions(REP);

BENCHMARK_MAIN();
