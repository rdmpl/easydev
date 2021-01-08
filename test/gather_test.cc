#include <benchmark/benchmark.h>
#include <fmt/format.h>
#include <easydev/gather.h>
#include <stdlib.h>
const int scale = 1000000;
void gather_test(benchmark::State &state){
    Gather g(scale + 1);
    int l = 0, r = 0;
    for(auto it : state){
       l = rand() % scale;
       r = rand() % scale;
       g.find(l);
       g.find(r);
       g.merge(l, r);
    }
}
BENCHMARK(gather_test);
BENCHMARK_MAIN();
