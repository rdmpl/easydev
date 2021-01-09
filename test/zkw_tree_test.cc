#include <benchmark/benchmark.h>
#include <easydev/zkw_tree.h>
#include <stdlib.h>
const static int scale = 1000000;
void zkw_tree_test(benchmark::State &s) {
  ZKWTree<int> t(scale);
  for (auto it : s) {
    int l = rand() % (scale - 1);
    int r = rand() % (scale - l) + l;
    t.update(l, r, 1);
    t.query(l, r);
  }
}
BENCHMARK(zkw_tree_test);
BENCHMARK_MAIN();