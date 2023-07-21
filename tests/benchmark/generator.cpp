#include "coroutine/generator.hpp"

#include <benchmark/benchmark.h>

static constexpr int ITER = 1000;

int f(int i) {
  // return std::floor(10 * std::sin(1 / (i * i)));
  return i;
}

Mylib::Coroutine::Generator<int> number() {
  for (int i = 0; i < ITER; i++) {
    co_yield f(i);
  }
}

void bare_for(benchmark::State &state) {
  for (auto _ : state) {
    for (int i = 0; i < ITER; i++) {
      benchmark::DoNotOptimize(f(i));
    }
  }
}

void for_with_generator(benchmark::State &state) {
  for (auto _ : state) {
    for (auto g = number(); g.next();) {
      benchmark::DoNotOptimize(g.value());
    }
  }
}

BENCHMARK(bare_for);
BENCHMARK(for_with_generator);

BENCHMARK_MAIN();
