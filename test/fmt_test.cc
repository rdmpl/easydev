#include <benchmark/benchmark.h>
#include <fmt/format.h>
void fmt_test(benchmark::State &s) {
  int i = 0;
  for (auto it : s) {
    fmt::print("i = {}.\n", i++);
  }
}
BENCHMARK(fmt_test)->Iterations(10000);
BENCHMARK_MAIN();
