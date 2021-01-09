#include <benchmark/benchmark.h>
#include <easydev/array_tree.h>
#include <stdlib.h>
const static int scale = 1000000;
void array_tree_test(benchmark::State &s) {
  ArrayTree<int> t(scale);
  for (auto it : s) {
    int l = rand() % (scale - 1);
    int r = rand() % (scale - l) + l;
    t.update(l, r, 1);
    t.query(l, r);
  }
}
BENCHMARK(array_tree_test);
BENCHMARK_MAIN();