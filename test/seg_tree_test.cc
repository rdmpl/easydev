#include <benchmark/benchmark.h>
#include <easydev/seg_tree.h>
#include <stdlib.h>
const static int scale = 1000000;
void seg_tree_test(benchmark::State &s) {
  SegmentTree<int> t(scale);
  for (auto it : s) {
    int l = rand() % (scale - 1);
    int r = rand() % (scale - l) + l;
    t.update(l, r, 1);
    t.query(l, r);
  }
}
BENCHMARK(seg_tree_test);
BENCHMARK_MAIN();