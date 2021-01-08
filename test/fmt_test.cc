#include <fmt/format.h>
#include <benchmark/benchmark.h>
void fmt_test(benchmark::State &s){
    int i = 0;
    for(auto it : s){
        fmt::print("i = {}.\n", i++);
    }
}
BENCHMARK(fmt_test);
BENCHMARK_MAIN();
