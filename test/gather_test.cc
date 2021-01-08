#include <benchmark/benchmark.h>
#include <fmt/format.h>
#include <easydev/gather.h>
#include <stdlib.h>
const int scale = 100000;
void gather_test(benchmark::State &state){
    Gather g(scale + 10);
    int l = 0, r = 0;
    int i = 0;
    for(auto it : state){
       l = i, r = i + 1;
       g.merge(r, l);
       g.find(i);
       g.find(i + 1);
       i = (i + 1) % (scale - 1);
    }
}
void gather_test2(benchmark::State &state){
    Gather g(scale + 10);
    int l = 0, r = 0, i = 0;
    for(auto it : state){
        l = i, r = i + 1;
        g.merge(l, r);
        g.find(i);
        g.find(i + 1);
        i = (i + 1) % (scale - 1);
    }
}

        
BENCHMARK(gather_test);
BENCHMARK(gather_test2);
BENCHMARK_MAIN();
